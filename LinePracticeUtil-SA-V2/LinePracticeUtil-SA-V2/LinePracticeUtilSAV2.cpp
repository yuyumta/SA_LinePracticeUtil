#include "plugin.h"
#include "CTimer.h"
#include "CHud.h"
#include "CCamera.h"
#include "extensions/ScriptCommands.h"
using namespace plugin;

class LinePracticeUtilSAV2 {
public:
    LinePracticeUtilSAV2() {

        static CVector position;
        static CVector speed;
        static CVector camera;
        static float ux, uy, uz, rx, ry, rz;
        static int m_nLastSpawnedTime = 0;
        static float cx, cy, cz;
        static float vhealth;

        Events::gameProcessEvent += [] {

            CVehicle* playerVehicle = FindPlayerVehicle(-1, false);
            CPhysical* entity = (CPhysical*)FindPlayerEntity(-1);
            CPlayerPed* player = FindPlayerPed(-1);

            if (playerVehicle && KeyPressed(0x31) && CTimer::m_snTimeInMilliseconds - m_nLastSpawnedTime > 500 && ux) {

                m_nLastSpawnedTime = CTimer::m_snTimeInMilliseconds;

                CVehicle* playerVehicle = FindPlayerVehicle(-1, false);
                CPhysical* entity = (CPhysical*)FindPlayerEntity(-1);
                CPlayerPed* player = FindPlayerPed(-1);
                entity->Teleport(position, false);

                CHud::SetHelpMessage("Loaded", true, false, false);
 
                player->m_nPedFlags.CantBeKnockedOffBike = 0; // so you can warp back to your savepoint after driving into water
                Command<eScriptCommands::COMMAND_WARP_CHAR_INTO_CAR>(player, playerVehicle); // so the sound starts working after you drive a bike into water

                playerVehicle->m_matrix->up.x = ux;
                playerVehicle->m_matrix->up.y = uy;
                playerVehicle->m_matrix->up.z = uz;
                playerVehicle->m_matrix->right.x = rx;
                playerVehicle->m_matrix->right.y = ry;
                playerVehicle->m_matrix->right.z = rz;
                playerVehicle->Fix();
                playerVehicle->m_fHealth = vhealth;
                playerVehicle->m_vecMoveSpeed = speed;
                Command<eScriptCommands::COMMAND_RESTORE_CAMERA_JUMPCUT>();
            }
            if (playerVehicle && KeyPressed(VK_TAB) && CTimer::m_snTimeInMilliseconds - m_nLastSpawnedTime > 500) {

                m_nLastSpawnedTime = CTimer::m_snTimeInMilliseconds;

                CVehicle* playerVehicle = FindPlayerVehicle(-1, false);
                CPhysical* entity = (CPhysical*)FindPlayerEntity(-1);
                CPlayerPed* player = FindPlayerPed(-1);

                CHud::SetHelpMessage("Set", true, false, false);
                

                position = entity->GetPosition();
                speed = playerVehicle->m_vecMoveSpeed;
                vhealth = playerVehicle->m_fHealth;

                ux = playerVehicle->m_matrix->up.x;
                uy = playerVehicle->m_matrix->up.y;
                uz = playerVehicle->m_matrix->up.z;
                rx = playerVehicle->m_matrix->right.x;
                ry = playerVehicle->m_matrix->right.y;
                rz = playerVehicle->m_matrix->right.z;
            }
            if (!playerVehicle && KeyPressed(0x31) && CTimer::m_snTimeInMilliseconds - m_nLastSpawnedTime > 500) {

                for (CVehicle* vehicle : CPools::ms_pVehiclePool)
                {
                    if (vehicle == player->m_pVehicle) {

                        player->m_nPedFlags.CantBeKnockedOffBike = 1; // water bike
                        Command<eScriptCommands::COMMAND_WARP_CHAR_INTO_CAR>(player, vehicle);

                    }
                }

            }

        };
    }
} linePracticeUtilSAV2;
