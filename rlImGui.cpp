#include "rlImGui.h"
#include "imgui.h"
#include "raylib.h"
#include "rlgl.h"

#include <map>
#include <string>

static Texture2D g_FontTexture;
static bool g_MousePressed[3] = { false, false, false };

static const char* rlImGuiGetClipText(void*)
{
    return GetClipboardText();
}

static void rlImGuiSetClipText(void*, const char* text)
{
    SetClipboardText(text);
}
static void rlImGuiNewFrame()
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2((float)GetScreenWidth(), (float)GetScreenHeight());
    io.DeltaTime = GetFrameTime();

    io.MouseDown[0] = IsMouseButtonDown(MOUSE_BUTTON_LEFT);
    io.MouseDown[1] = IsMouseButtonDown(MOUSE_BUTTON_RIGHT);
    io.MouseDown[2] = IsMouseButtonDown(MOUSE_BUTTON_MIDDLE);

    Vector2 mousePosition = GetMousePosition();
    io.MousePos = ImVec2(mousePosition.x, mousePosition.y);
    // -----------------------

    io.MouseWheel += GetMouseWheelMove();

    io.KeyCtrl = IsKeyDown(KEY_RIGHT_CONTROL) || IsKeyDown(KEY_LEFT_CONTROL);
    io.KeyShift = IsKeyDown(KEY_RIGHT_SHIFT) || IsKeyDown(KEY_LEFT_SHIFT);
    io.KeyAlt = IsKeyDown(KEY_RIGHT_ALT) || IsKeyDown(KEY_LEFT_ALT);
    io.KeySuper = IsKeyDown(KEY_RIGHT_SUPER) || IsKeyDown(KEY_LEFT_SUPER);

    int key = GetKeyPressed();
    while (key > 0)
    {
        io.AddInputCharacter(key);
        key = GetKeyPressed();
    }
}
static void rlImGuiRender(ImDrawData* draw_data)
{
    rlDrawRenderBatchActive();
    rlDisableBackfaceCulling();

    for (int n = 0; n < draw_data->CmdListsCount; n++)
    {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
        {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];

            if (pcmd->UserCallback)
            {
                pcmd->UserCallback(cmd_list, pcmd);
            }
            else
            {
                rlBegin(RL_TRIANGLES);
                rlSetTexture((unsigned int)(uintptr_t)pcmd->TextureId);

                for (unsigned int i = 0; i < pcmd->ElemCount; i++)
                {
                    const ImDrawVert& vert = cmd_list->VtxBuffer[cmd_list->IdxBuffer[pcmd->IdxOffset + i]];

                    unsigned char r = (vert.col >> 0) & 0xFF;
                    unsigned char g = (vert.col >> 8) & 0xFF;
                    unsigned char b = (vert.col >> 16) & 0xFF;
                    unsigned char a = (vert.col >> 24) & 0xFF;
                    rlColor4ub(r, g, b, a);

                    rlTexCoord2f(vert.uv.x, vert.uv.y);
                    rlVertex2f(vert.pos.x, vert.pos.y);
                }
                rlEnd();
            }
        }
    }

  //  rlEnableBackfaceCulling();
}

void rlImGuiSetup(bool dark)
{
    ImGui::CreateContext();
    if (dark)
        ImGui::StyleColorsDark();
    else
        ImGui::StyleColorsLight();

    ImGuiIO& io = ImGui::GetIO();
    io.BackendPlatformName = "imgui_impl_raylib";


    io.SetClipboardTextFn = rlImGuiSetClipText;
    io.GetClipboardTextFn = rlImGuiGetClipText;
    io.ClipboardUserData = NULL;

    unsigned char* pixels;
    int width, height;
    io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);
    Image image = { pixels, width, height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
    g_FontTexture = LoadTextureFromImage(image);
    io.Fonts->TexID = (ImTextureID)(uintptr_t)g_FontTexture.id;
}

void rlImGuiShutdown()
{
    UnloadTexture(g_FontTexture);
    ImGui::DestroyContext();
}

void rlImGuiBegin()
{
    rlImGuiNewFrame();
    ImGui::NewFrame();
}

void rlImGuiEnd()
{
    ImGui::Render();
    rlImGuiRender(ImGui::GetDrawData());
}

void rlImGuiImage(const Texture* image)
{
    ImGui::Image((ImTextureID)(uintptr_t)image->id, ImVec2((float)image->width, (float)image->height));
}