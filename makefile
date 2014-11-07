DUMMY=--[[

###############################################################
# Move this to project folder and run it to use the framework #
###############################################################

build:
	@lua5.1 ./makefile build

run:
	@lua5.1 ./makefile run

package:
	@lua5.1 ./makefile package

ifeq (0, 1)

# ]]

require "lfs"
regex = require "rex_posix"

-- http://keplerproject.github.io/luafilesystem/manual.html
-- http://rrthomas.github.io/lrexlib/manual.html

local params = {...}

local projectName = regex.gmatch(lfs.currentdir(), "([A-Za-z0-9_]+)$")()
local platform = "linux"

local compiler = "g++"
local linker = "g++"

local libraries = ""
if platform == "linux" then
	libraries = "-lGL -lsfml-graphics -lsfml-window -lsfml-system"
elseif platform == "win32" then
	compiler = "i686-w64-mingw32-g++"
	linker = "i686-w64-mingw32-g++"
	libraries = "-static-libstdc++ -static-libgcc -static -lwinpthread "
	libraries = libraries.."-lsfml-window -lsfml-system -lsfml-graphics"
	-- libraries = libraries.." -lSDL2_ttf -lSDL2_image -llua -lBox2D" -- BOX2D DOESN'T EXIST YET
end

local commonflags = "-I./ -std=c++11 -Wall"
local compileflags = "-DGLEW_STATIC -DPROJECTNAME=\\\""..projectName.."\\\""
local linkflags = "-Wl,-rpath=./libs/"..platform.."/"

local buildName = projectName
if platform == "win32" then
	buildName = buildName..".exe"
end

local runCommand = "./"..buildName

if platform == "win32" then
	compileflags = compileflags.." -D_WIN32_WINNT=0x0501 -D_WIN32 -DWIN32 -mwindows"
	runCommand = "wine "..buildName
end

runCommand = "gnome-terminal -e \""..runCommand.."\""
-- runCommand = "optirun "..runCommand
-- runCommand = "vblank_mode=0 "..runCommand

function getFilesInDir(root)
	local files = {}

	for entity in lfs.dir(root) do
		if entity~="." and entity~=".." then
			local fullPath=root.."/"..entity
			local mode=lfs.attributes(fullPath,"mode")
			if mode=="file" then
				files[#files+1] = fullPath
			elseif mode=="directory" then
				local morefiles = getFilesInDir(fullPath)

				for _,v in pairs(morefiles) do
					files[#files+1] = v
				end
			end
		end
	end

	return files
end

function getFilesOfType(type)
	local files = getFilesInDir(".")

	for k,v in pairs(files) do
		if not regex.match(v,type) then
			files[k] = nil
		end
	end

	local i = 1

	while i < #files do
		if not files[i] then
			table.remove(files, i)
		else
			i = i + 1
		end
	end

	return files
end

function toObjectPath(name)
	name = regex.gsub(name, "/", "_")
	return "./obj/"..platform.."/"..name..".o"
end

function checkSourceModificationTimes()
	local toBeRebuilt = {}

	for k,v in pairs(getFilesOfType("c")) do
		-- print(k,v,mod)
		local name = regex.gmatch(v, "\./([a-zA-Z_/]+)\.c$")()
		if name then
			-- print(v, name)
			local cmod = lfs.attributes(v, "modification")
			local omod = lfs.attributes(toObjectPath(name), "modification")

			if not omod or omod < cmod then
				print(name.." "..string.rep(".", 20-#name).." needs to be rebuilt")
				toBeRebuilt[#toBeRebuilt+1] = {name=name, ext=".c"} 
			else
				print(name.." "..string.rep(".", 20-#name).." up to date")
			end
		end
	end

	for k,v in pairs(getFilesOfType("(cpp|h)")) do
		-- print(k,v,mod)
		local name = regex.gmatch(v, "\./([a-zA-Z_/]+)\.cpp$")()
		if name then
			-- print(v, name)
			local cmod = lfs.attributes(v, "modification")
			local omod = lfs.attributes(toObjectPath(name), "modification")
			local hmod = lfs.attributes(name..".h", "modification")

			if not omod or omod < cmod or hmod and omod < hmod then
				print(name.." "..string.rep(".", 20-#name).." needs to be rebuilt")
				toBeRebuilt[#toBeRebuilt+1] = {name=name, ext=".cpp"} 

			elseif cmod and (omod-cmod) > 1000 or hmod and (omod-hmod) > 1000 then
				print(name.." "..string.rep(".", 20-#name).." object file newer than source")

				local removecmd = "rm -f "..toObjectPath(name)
				local touchcmd = "touch ./"..name..".cpp"
				local touchhcmd = "touch ./"..name..".h"

				os.execute(removecmd)
				os.execute(touchcmd)
				if(hmod) then os.execute(touchhcmd) end

				toBeRebuilt[#toBeRebuilt+1] = {name=name, ext=".cpp"} 
			else
				print(name.." "..string.rep(".", 20-#name).." up to date")
			end
		end
	end

	io.flush()

	return toBeRebuilt
end

function createDefaultDirs()
	local defaultdirs = {
		"obj",
		"obj/linux",
		"obj/win32",
		"libs",
		"libs/linux",
		"libs/win32"
	}

	for _,v in pairs(defaultdirs) do
		lfs.mkdir(v)
	end
end

function rebuildObj(toBeRebuilt, ext)
	ext = ext or ".cpp"

	print("Building ............ "..toBeRebuilt)

	local objpath = toObjectPath(toBeRebuilt)
	io.flush()
	os.execute(compiler.." -c "..toBeRebuilt..ext.." "..compileflags.." "..commonflags.." -o "..objpath)
end

function rebuildExec()
	local objFileList = toObjectPath("*")

	io.flush()
	os.execute(compiler.." -o "..buildName.." "..objFileList.." "..commonflags.." "..linkflags.." "..libraries)
end

function build()
	print("Building "..projectName.." for "..platform.."...\n")
	io.flush()
	createDefaultDirs()
	local toBeRebuilt = checkSourceModificationTimes()
	print()
	for k,v in pairs(toBeRebuilt) do
		rebuildObj(v.name, v.ext)
	end

	rebuildExec()
end

function run()
	local smod = checkSourceModificationTimes()
	if #smod > 0 then
		createDefaultDirs()
		for k,v in pairs(smod) do
			rebuildObj(v.name, v.ext)
		end

		rebuildExec()
	end

	print("\nRunning ...\n")

	io.flush()

	if platform == "win32" then
		os.execute("cp libs/win32/* .")
	end

	os.execute(runCommand)

	if platform == "win32" then
		os.execute("sleep 1")
		os.execute("rm *.dll")
	end
end

function package()
	local assets = buildName.." ./libs/win32/* *.ttf images"
	if platform == "win32" then
		os.execute("mkdir "..projectName.."_win32")
		os.execute("cp -r "..assets.." ./"..projectName.."_win32")
		os.execute("zip -mr "..projectName..".zip ./"..projectName.."_win32/*")
		os.execute("rm -rf ./"..projectName.."_win32")
	end
end

if params[1] == "build" then
	build()
elseif params[1] == "run" then
	run()
elseif params[1] == "package" then
	package()
end

--[[

endif

#]]
