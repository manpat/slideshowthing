#ifndef EVENTSYSTEM_H
#define EVENTSYSTEM_H

// Signal + Slots mechanism

/* Three possible slot callbacks:
 *		- Global/Static functions
 *		- Lambdas
 *		- Member functions (actually lambdas)
 */

#include <list>
#include <functional>
#include <memory>
#include <cstdint>

typedef uint64_t SlotUUID_t;

static SlotUUID_t GetSlotUUID(){
	static SlotUUID_t uuid = 0;
	return ++uuid;
}

template<typename... CallbackArgs> class Slot;

namespace secret{
	template<typename... CallbackArgs> class Connection;
}

template<typename... CallbackArgs>
class Signal {
	friend Slot<CallbackArgs...>;
protected:
	typedef secret::Connection<CallbackArgs...> ConnectionType;

	std::list<std::shared_ptr<Slot<CallbackArgs...>>> staticSlots;
	std::shared_ptr<ConnectionType> firstSlot; // Linked list
	std::shared_ptr<ConnectionType> lastSlot;

	void RemoveConnection(std::shared_ptr<ConnectionType>);

public:
	Signal();
	~Signal();

	void Connect(Slot<CallbackArgs...>&);
	void Connect(Slot<CallbackArgs...>&&);
	void Disconnect(Slot<CallbackArgs...>&);
	void DisconnectAll();

	void Raise(CallbackArgs...);
};

template<typename... CallbackArgs>
class Slot {
	friend Signal<CallbackArgs...>;
protected:
	typedef secret::Connection<CallbackArgs...> ConnectionType;

	std::list<std::weak_ptr<ConnectionType>> connections;
	std::function<void (CallbackArgs...)> callback;
	SlotUUID_t uuid;

public:
	Slot();
	Slot(const Slot<CallbackArgs...>&);
	Slot(Slot<CallbackArgs...>&&);
	Slot(std::function<void (CallbackArgs...)>);
	Slot<CallbackArgs...>& operator=(const Slot<CallbackArgs...>&);
	Slot<CallbackArgs...>& operator=(Slot<CallbackArgs...>&&);
	~Slot();

	void Disconnect(); // Destroys all connections

	template<class C>
	static Slot<CallbackArgs...> Member(void (C::*)(CallbackArgs...), C*);

	void Raise(CallbackArgs...);

	bool operator==(Slot<CallbackArgs...>&);
};

namespace secret {
	template<typename... CallbackArgs>
	class Connection {
	public:
		std::shared_ptr<Connection<CallbackArgs...>> next;
		std::shared_ptr<Connection<CallbackArgs...>> prev;

		Signal<CallbackArgs...>* signal;
		Slot<CallbackArgs...>* slot;

	public:
		Connection();
		~Connection();

		void Disconnect();
		void Raise(CallbackArgs...);
	};
};

#include "eventsystem.inl"

#endif