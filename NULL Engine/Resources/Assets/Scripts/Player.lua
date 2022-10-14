-- Global Variables
local Utils = Event.Utils ()

-- Function Declarations
---
function GetTable_Player()

	lua_table = {}
	lua_table["MovementSpeed"] = 25.0
	lua_table["RotationSpeed"] = 5.0

		---
		function lua_table:Start()

			Utils:LOG("Player script has started!");
		
		end
	
		---
		function lua_table:Update()

			dt = Utils:GetGameDT()
			if Utils:IsKeyRepeat("W") then Utils:Translate(0.0, 0.0, lua_table["MovementSpeed"] * dt) Utils:LOG("Going Forward!") end
			if Utils:IsKeyRepeat("A") then Utils:Translate(-lua_table["MovementSpeed"] * dt, 0.0, 0.0) Utils:LOG("Going Left!") end
			if Utils:IsKeyRepeat("S") then Utils:Translate(0.0, 0.0, -lua_table["MovementSpeed"] * dt) Utils:LOG("Going Backwards!") end
			if Utils:IsKeyRepeat("D") then Utils:Translate(lua_table["MovementSpeed"] * dt, 0.0, 0.0) Utils:LOG("Going Right!") end
			
			if Utils:IsKeyRepeat("Q") then Utils:Rotate(0.0, lua_table["RotationSpeed"] * dt, 0.0) Utils:LOG("Rotating Left!") end
			if Utils:IsKeyRepeat("E") then Utils:Rotate(0.0, -lua_table["RotationSpeed"] * dt, 0.0) Utils:LOG("Rotating Right!") end
		
		end
	
	return lua_table
end

