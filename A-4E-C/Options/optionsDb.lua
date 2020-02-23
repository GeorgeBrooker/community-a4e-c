local DbOption  = require('Options.DbOption')
local i18n	    = require('i18n')
local oms       = require('optionsModsScripts')

local _ = i18n.ptranslate


-- find the relative location of optionsDb.lua
function script_path() 
    -- remember to strip off the starting @ 
	local luafileloc = debug.getinfo(2, "S").source:sub(2)
	local ti, tj = string.find(luafileloc, "Options")
	local temploc = string.sub(luafileloc, 1, ti-1)
    return temploc
end 

-- find module path
local relativeloc = script_path()
modulelocation = lfs.currentdir().."\\"..relativeloc

local tblCPLocalList = oms.getTblCPLocalList(modulelocation)


return {

	rwrType					= DbOption.new():setValue(0):combo({DbOption.Item(_('AN/APR-23 (Sound only)')):Value(0),
																DbOption.Item(_('AN/APR-25 (Display)')):Value(1),
																--[[DbOption.Item(_('ON')):Value(2),
																DbOption.Item(_('DISMOUNTED')):Value(3),]]--
																}),

	trimSpeed				= DbOption.new():setValue(0):combo({DbOption.Item(_('100% (default)')):Value(0),
																DbOption.Item(_('50%')):Value(1),																
																}),

	catapultAlignmentCheck	= DbOption.new():setValue(true):checkbox(),
}
