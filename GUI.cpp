#include "GUI.h"
#include "ScreenShot.h"

void GUI::init(GLFWwindow* window) {
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	io = & ImGui::GetIO(); (void)io;
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	const char* glsl_version = "#version 130";
	ImGui_ImplOpenGL3_Init(glsl_version);
	io->Fonts->AddFontDefault();
}
void GUI::newframe() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}
void GUI::draw(Render* render) {

    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

        static float lightPos[3] = {0.0f, 1.0f, 3.0f};
        static float lightTarget[3];
        static float lightIntensity=10.0f;
        if (ImGui::CollapsingHeader("Light configuration:"))
        {

            ImGui::Text("Position of light:");
            ImGui::SliderFloat("x###lightPosX", &lightPos[0], 0.0f, 5.0f);
            ImGui::SliderFloat("y###lightPosY", &lightPos[1], 0.0f, 5.0f);
            ImGui::SliderFloat("z###lightPosZ", &lightPos[2], 0.0f, 5.0f);
            ImGui::Text("Target of light:");
            ImGui::SliderFloat("x###lightTargetX", &lightTarget[0], 0.0f, 5.0f);
            ImGui::SliderFloat("y###lightTargetY", &lightTarget[1], 0.0f, 5.0f);
            ImGui::SliderFloat("z###lightTargetZ", &lightTarget[2], 0.0f, 5.0f);
            ImGui::Text("Intensity of light:");
            ImGui::SliderFloat("Intensity###lightIntensity", &lightIntensity, 0.0f, 10.0f);
        }
        render->updateLight(vec3(lightPos[0], lightPos[1], lightPos[2]), vec3(lightTarget[0], lightTarget[1], lightTarget[2]), lightIntensity);
        static const char* items[]{ "Item1","Item2","Item3","Item4","Item5","Item6","Item7","Item8","Item9" };
        static int selectedItem = 0;
        static const int numOfItems = 9;
        static float Scale[numOfItems] = { 1.0f, 1.0f, 1.0f };
        static float translate[numOfItems][3] = {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f}
        };
        static float rotation[numOfItems][3];
        static int textureMapID = 0;
        /*   imgui::listbox("listbox", &selecteditem, items, im_arraysize(items),1);
           imgui::text(items[selecteditem]);*/

        if (ImGui::CollapsingHeader("Items configuration:"))
        {
            ImGui::Combo("ListBox", &selectedItem, items, IM_ARRAYSIZE(items));
            ImGui::Text("Translation of %s", items[selectedItem]);
            ImGui::SliderFloat("x###translationX", &translate[selectedItem][0], 0.0f, 1.0f);
            ImGui::SliderFloat("y###translationY", &translate[selectedItem][1], 0.0f, 1.0f);
            ImGui::SliderFloat("z###translationZ", &translate[selectedItem][2], 0.0f, 1.0f);
            ImGui::Text("Rotation of %s", items[selectedItem]);
            ImGui::SliderFloat("x###rotationX", &rotation[selectedItem][0], 0.0f, 360.0f);
            ImGui::SliderFloat("y###rotationY", &rotation[selectedItem][1], 0.0f, 360.0f);
            ImGui::SliderFloat("z###rotationZ", &rotation[selectedItem][2], 0.0f, 360.0f);
            ImGui::Text("Scale of %s", items[selectedItem]);
            ImGui::SliderFloat("Scale", &Scale[selectedItem], 0.0f, 10.0f);
            if (selectedItem == 0)
            {
                ImGui::Text("Texture of %s", items[selectedItem]);
                ImGui::SliderInt("textureMapID###textureMapID", &textureMapID, 0, 39);
                render->updatetextureMapID(textureMapID);
            }
            static mat4 NewWorldMatrix= glm::identity<mat4>();
            static mat4 TransMatrix = glm::identity<mat4>();
            static mat4 RotateMatrix = glm::identity<mat4>();
            TransMatrix= glm::translate(glm::mat4(1.0f), glm::vec3(translate[selectedItem][0], translate[selectedItem][1], translate[selectedItem][2]));
            RotateMatrix= glm::rotate(TransMatrix, glm::radians(rotation[selectedItem][0]), glm::vec3(1.0f, 0.0f, 0.0f));
            RotateMatrix= glm::rotate(RotateMatrix, glm::radians(rotation[selectedItem][1]), glm::vec3(0.0f, 1.0f, 0.0f));
            RotateMatrix= glm::rotate(RotateMatrix, glm::radians(rotation[selectedItem][2]), glm::vec3(0.0f, 0.0f, 1.0f));
            NewWorldMatrix = glm::scale(RotateMatrix, glm::vec3(Scale[selectedItem], Scale[selectedItem], Scale[selectedItem]));
            render->updateItemWorldMatrix(selectedItem, NewWorldMatrix);
        }
        static bool cameraOrbit = false;
        static float angle1 = acos(1/sqrt(3)), angle2 = acos(1/sqrt(2)), dist = 5 * sqrt(3);
        if (ImGui::CollapsingHeader("Camera configuration:"))
        {
            ImGui::Checkbox("Orbit", &cameraOrbit);
            //if (cameraOrbit == true) cameraOrbit = false;
            if (cameraOrbit) {
                    printf("Orbiting\n");
                    angle2 += 0.01;
                    if (angle2 > 6.28)
                            angle2 -= 6.28;
                    render->updateCameraPos(vec3(dist * sin(angle1) * cos(angle2), dist * cos(angle1), dist * sin(angle1) * sin(angle2)));
                   // render->updateCameraPos(vec3(-3, -3, 3));
                    render->updateCameraLookAt(vec3(0, 0, 0));
            }

            ImGui::SameLine();
            if (ImGui::Button("ZoomToFit")) {
                    cameraOrbit = false;
                    render->updateCameraPos(vec3(3, 3, 3));
                    render->updateCameraLookAt(vec3(0, 0, 0));
            }
        }
        static char meshFilename[64] = "dummy.obj";
        static char screenShotFilename[64] = "dummy.bmp";
        if (ImGui::CollapsingHeader("Export to file:"))
        {
            ImGui::Text("Export mesh:");
            ImGui::InputText("File name###meshFilename", meshFilename, IM_ARRAYSIZE(meshFilename));
            ImGui::SameLine();
            if (ImGui::Button("export###mesh"))
            {
                render->updateExpObj(1, meshFilename);
            }
            else {
                render->updateExpObj(0, "dummy");
            }

            ImGui::Text("Screen shot:");
            ImGui::InputText("File name###screenShotFilename", screenShotFilename, IM_ARRAYSIZE(screenShotFilename));
            ImGui::SameLine();
            if (ImGui::Button("export###img"))
            {
                    printf("Printing\n");
                    bool flag_shot = ScreenShot(720/*height*/, 1280/*width*/, screenShotFilename);
                    if (flag_shot)
                            printf("Export succeeded. \n");
                    else
                            printf("Export failed. \n");

            }
        }
        ImGui::End();
    }

    ImGui::Render();
}
void GUI::RenderDrawData() {
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
void GUI::clearup() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
