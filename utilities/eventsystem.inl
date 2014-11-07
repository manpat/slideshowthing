#ifndef EVENTSYSTEM_INL
#define EVENTSYSTEM_INL

template<typename... CallbackArgs>
Signal<CallbackArgs...>::Signal(){
	firstSlot = nullptr;
	lastSlot = nullptr;
}

template<typename... CallbackArgs>
Signal<CallbackArgs...>::~Signal(){
	DisconnectAll();
}

template<typename... CallbackArgs>
void Signal<CallbackArgs...>::Connect(Slot<CallbackArgs...>& slot){
	using std::shared_ptr;
	using std::make_shared;

	shared_ptr<ConnectionType> con = make_shared<ConnectionType>();

	if(!firstSlot){
		firstSlot = con;
		lastSlot = con;
	}else{
		lastSlot->next = con;
		con->prev = lastSlot;
		lastSlot = con;
	}

	con->signal = this;
	con->slot = &slot;

	slot.connections.push_back(con);
}
template<typename... CallbackArgs>
void Signal<CallbackArgs...>::Connect(Slot<CallbackArgs...>&& _slot){
	using std::shared_ptr;
	using std::make_shared;

	shared_ptr<Slot<CallbackArgs...>> slot = make_shared<Slot<CallbackArgs...>>(_slot);
	
	Connect(*slot);

	staticSlots.push_back(slot);
}

template<typename... CallbackArgs>
void Signal<CallbackArgs...>::Disconnect(Slot<CallbackArgs...>& slot){
	auto c = firstSlot;
	while(c){
		if(*c->slot == slot){
			RemoveConnection(c);
		}

		c = c->next;
	}
}
template<typename... CallbackArgs>
void Signal<CallbackArgs...>::DisconnectAll(){
	auto c = lastSlot;
	while(c && c->prev){
		c = c->prev;
		if(c) c->next->Disconnect();
	}
	if(c) c->Disconnect();

	firstSlot = lastSlot = nullptr;
	staticSlots.clear();
}
template<typename... CallbackArgs>
void Signal<CallbackArgs...>::RemoveConnection(std::shared_ptr<ConnectionType> c){
	if(c == firstSlot) firstSlot = c->next;
	if(c == lastSlot) lastSlot = c->prev;

	c->Disconnect();
}

template<typename... CallbackArgs>
void Signal<CallbackArgs...>::Raise(CallbackArgs... args){
	std::shared_ptr<ConnectionType> con = firstSlot;
	while(con){
		con->Raise(args...);
		con = con->next;
	}
}

template<typename... CallbackArgs>
Slot<CallbackArgs...>::Slot()
	: callback(nullptr), uuid(0){}

template<typename... CallbackArgs>
Slot<CallbackArgs...>::Slot(const Slot<CallbackArgs...>& s)
	: callback(s.callback), uuid(s.uuid){}

template<typename... CallbackArgs>
Slot<CallbackArgs...>::Slot(Slot<CallbackArgs...>&& s)
	: callback(std::move(s.callback)), uuid(s.uuid){
		s.callback = nullptr;
		s.uuid = 0;
	}
template<typename... CallbackArgs>
Slot<CallbackArgs...>::Slot(std::function<void (CallbackArgs...)> _callback)
	: callback(_callback), uuid(GetSlotUUID()){}

template<typename... CallbackArgs>
Slot<CallbackArgs...>::~Slot(){
	Disconnect();
}

template<typename... CallbackArgs>
void Slot<CallbackArgs...>::Disconnect(){
	for(auto wc : connections){
		if(auto con = wc.lock()){
			if(con->signal){
				con->signal->RemoveConnection(con);
			}else{
				// Debug::Warning("Removed orphaned Signal-Slot Connection");
				con->Disconnect();
			}
		}
	}

	connections.clear();
}

template<typename... CallbackArgs>
Slot<CallbackArgs...>& Slot<CallbackArgs...>::operator=(const Slot<CallbackArgs...>& s){
	callback = s.callback;
	uuid = s.uuid;
	return *this;
}
template<typename... CallbackArgs>
Slot<CallbackArgs...>& Slot<CallbackArgs...>::operator=(Slot<CallbackArgs...>&& s){
	callback = s.callback;
	uuid = s.uuid;

	s.callback = nullptr;
	s.uuid = 0;
	return *this;
}

template<typename... CallbackArgs>
template<typename C>
Slot<CallbackArgs...> Slot<CallbackArgs...>::Member(void (C::*fn)(CallbackArgs...), C* o){
	return Slot<CallbackArgs...>([=](CallbackArgs... a){
		(o->*fn)(a...); // Hacky but whatever
	});
}


template<typename... CallbackArgs>
void Slot<CallbackArgs...>::Raise(CallbackArgs... args){
	if(uuid != 0 && callback)
		callback(args...);
}

template<typename... CallbackArgs>
bool Slot<CallbackArgs...>::operator==(Slot<CallbackArgs...>& s){
	return uuid == s.uuid;
}

namespace secret {
	template<typename... CallbackArgs>
	Connection<CallbackArgs...>::Connection(){
		// These are initiallised externally
		next = nullptr;
		prev = nullptr;

		signal = nullptr;
		slot = nullptr;
	}

	template<typename... CallbackArgs>
	Connection<CallbackArgs...>::~Connection(){
		Disconnect(); // Just in case
	}

	template<typename... CallbackArgs>
	void Connection<CallbackArgs...>::Disconnect(){
		// Orphan self
		// Because these are shared_ptrs, this should effectively 
		//	delete this object. The only time this doesn't happen is
		//	when the connection is first or last in a signal's list.
		//	But this is handled with RemoveConnection in Signal
		if(next) next->prev = prev;
		if(prev) prev->next = next;

		next = prev = nullptr;
	}

	template<typename... CallbackArgs>
	void Connection<CallbackArgs...>::Raise(CallbackArgs... args){
		if(slot) slot->Raise(args...);
	}
}

#endif