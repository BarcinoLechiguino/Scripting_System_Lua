-- Global Variables
--local Debug = Event.Debug ()

-- Function Declarations
---
function GetTable_Test()

	lua_table = {}
	lua_table["var"] = 0
	lua_table["Utils"] = Event.Utils ()
	
		---
		function lua_table:Start()
		
			lua_table["var"] = 11
			lua_table["Utils"]:LOG("Modified { var }! var = " .. lua_table["var"]);;
		
		end
	
		---
		function lua_table:Update()

			if lua_table["Utils"]:IsKeyDown("W") then lua_table["Utils"]:LOG("PRESSED W KEY!") end
			if lua_table["Utils"]:IsKeyDown("A") then lua_table["Utils"]:LOG("PRESSED A KEY!") end
			if lua_table["Utils"]:IsKeyDown("S") then lua_table["Utils"]:LOG("PRESSED S KEY!") end
			if lua_table["Utils"]:IsKeyDown("D") then lua_table["Utils"]:LOG("PRESSED D KEY!") end
		
		end
	
	return lua_table
end

------------------
--start_test = 1;
--update_test = 1;

---
--function Start()
--
--start_test = 0;
--update_test = 0;
--
--start_test = start_test + 5;
--
--LOG:LOG ("Lua Script: Test Log from test.lua script!");
--
--end
--
-----
--function Update()
--
--update_test = update_test + 1;
--
--end