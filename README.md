# WinRegTool
## Debug function list
![image](https://user-images.githubusercontent.com/10304041/168095561-1fe83821-97a9-45b4-ba09-bad4d44ecc17.png)
* Debug functions are disabled (0x00000000) by default. 
* All debug functions, by-pass, watermark, log, and dump, can be enabled or disabled individually.

## Registry path
Customer create registry manually
“HKEY_USER” ->” S-1-5-19”->” SOFTWARE”-> “alPPDebug”
“alPP_debug_flag”	(REG_DWORD)
“alPP_dump_path”	(REG_SZ)
![image](https://user-images.githubusercontent.com/10304041/168095716-8d2284d0-3cae-4860-9002-041e7b0272bc.png)

## alPP_WinRegTool.exe
Execute “alPP_WinRegTool.exe” for help
![image](https://user-images.githubusercontent.com/10304041/168095881-2ba3ba03-4425-4e6b-9050-c10fb0f9682b.png)

## alPP_WinRegTool.exe – mode 0
mode 0 : read alPP debug registry if it exist.
![image](https://user-images.githubusercontent.com/10304041/168096008-0296f515-6944-468a-a1cf-d60c36ec891f.png)
Not exist
![image](https://user-images.githubusercontent.com/10304041/168096032-ea83b701-daab-4780-a433-ee7e56e28dc2.png)

## alPP_WinRegTool.exe – mode 1
mode 1 : write alPP debug registry by alPP_DebugFunc.xml. If you don't have this file, please check it with Altek.
![image](https://user-images.githubusercontent.com/10304041/168096732-c714dcfa-d4fe-4e2a-affb-51b26da09c4d.png)
Modify alPP_DebugFunc.xml
![image](https://user-images.githubusercontent.com/10304041/168096825-c23da485-2d32-491b-9fe3-865a3aafee0a.png)

Execute alPP_WinRegTool.exe to write.
![image](https://user-images.githubusercontent.com/10304041/168096999-f6377d03-0c50-4840-982b-e5531819c49e.png)

alPP debug registry is set.
![image](https://user-images.githubusercontent.com/10304041/168097117-fb98f527-4dea-476b-b3bd-ed205a9e7e7f.png)

## alPP_WinRegTool.exe – mode 2
mode 2 : delete alPP debug registry
![image](https://user-images.githubusercontent.com/10304041/168097291-e4d6f5e5-956e-40f1-8df7-6efd7c7ab425.png)
alPP debug flag is deleted.
![image](https://user-images.githubusercontent.com/10304041/168097341-81c84cab-e618-47d2-8c0a-f4c379a3c70e.png)


