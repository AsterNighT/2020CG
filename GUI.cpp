#include "GUI.h"


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
    mycameraOrbit = false;
    myfreeViewpoint = false;
}
void GUI::newframe() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

}
std::string GUI::draw(Render* render) {
    std::string re;
    std::string tmpscreenShotFilename;
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
        //static char* items[]{ "Item1","Item2","Item3","Item4","Item5","Item6","Item7","Item8","Item9" };
        static const int MAXNumOfItems = 100;
        static char* itemsNamePointer[] = { "Item1","Item2","Item3","Item4","Item5","Item6","Item7","Item8","Item9" };
        
        static char nameBuffer[MAXNumOfItems][200];
        static int nameBufferID = 0;
        //itemsNamePointer[0] = &items[0][0];
        //itemsNamePointer[1] = &items[1][0];
        //itemsNamePointer[2] = &items[2][0];
        //items[2] = "fsdf";
        //items[3] = NULL;
        static int selectedItem = 0;
        
        static int numberOfItems = 4; //initial items in the scene 
        static float Scale[MAXNumOfItems] = { 1.0f,1.0f,1.0f,1.0f };
        static float translate[MAXNumOfItems][3] = {
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f},
            {0.0f, 0.0f, 0.0f}
        };
        static float rotation[MAXNumOfItems][3];
        static int textureMapID[MAXNumOfItems];
        //static int textureMapID = 0;
        /*   imgui::listbox("listbox", &selecteditem, items, im_arraysize(items),1);
           imgui::text(items[selecteditem]);*/
        
        static char ItemRename[64] = "";

        if (ImGui::CollapsingHeader("Items configuration:"))
        {   
            

            ImGui::Combo("ListBox", &selectedItem, itemsNamePointer, numberOfItems);
            ImGui::InputText("###renameText", ItemRename, IM_ARRAYSIZE(ItemRename));
            ImGui::SameLine();
            if (ImGui::Button("rename###renameButton")) {
               
                strcpy(nameBuffer[selectedItem], ItemRename);
                itemsNamePointer[selectedItem] = nameBuffer[selectedItem];
                render->updateItemName(selectedItem, ItemRename);
                ItemRename[0] = '\0';
            }
            ImGui::Text("Translation of %s", itemsNamePointer[selectedItem]);
            ImGui::SliderFloat("x###translationX", &translate[selectedItem][0], 0.0f, 1.0f);
            ImGui::SliderFloat("y###translationY", &translate[selectedItem][1], 0.0f, 1.0f);
            ImGui::SliderFloat("z###translationZ", &translate[selectedItem][2], 0.0f, 1.0f);
            ImGui::Text("Rotation of %s", itemsNamePointer[selectedItem]);
            ImGui::SliderFloat("x###rotationX", &rotation[selectedItem][0], 0.0f, 360.0f);
            ImGui::SliderFloat("y###rotationY", &rotation[selectedItem][1], 0.0f, 360.0f);
            ImGui::SliderFloat("z###rotationZ", &rotation[selectedItem][2], 0.0f, 360.0f);
            ImGui::Text("Scale of %s", itemsNamePointer[selectedItem]);
            ImGui::SliderFloat("Scale", &Scale[selectedItem], 0.0f, 10.0f);
            ImGui::Text("Texture of %s", itemsNamePointer[selectedItem]);
            ImGui::SliderInt("textureMapID###textureMapID", &textureMapID[selectedItem], 0, 40);
            render->updatetextureMapID(selectedItem,textureMapID[selectedItem]);  
            
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

        static bool freeViewpoint = false;

        static char ImportMeshFilename[64] = "in.obj";
        if (ImGui::CollapsingHeader("Import an obj:"))
        {
            freeViewpoint = false;
            ImGui::InputText("File name###ImportMeshFilename", ImportMeshFilename, IM_ARRAYSIZE(ImportMeshFilename));
            ImGui::SameLine();
            if (ImGui::Button("import##importObj")) {
                static std::vector<std::string> vectorOfItemName;
                vectorOfItemName = render->ImportItems(ImportMeshFilename);

                for (int i = 0; i < vectorOfItemName.size(); i++) {
                    static int ItemID;
                    ItemID= (numberOfItems++);
                    std::cout <<"ItemID"<< ItemID<<": "<<vectorOfItemName[i] << std::endl;
                    strcpy(nameBuffer[ItemID],vectorOfItemName[i].c_str());
                    itemsNamePointer[ItemID] = nameBuffer[ItemID];
                    Scale[ItemID] = 1.0f;
                }
                std::cout << vectorOfItemName.size();
                std::cout << "current number of items: " << numberOfItems << std::endl;
            }
        }

        static bool cameraOrbit = false;
        
        static float angle1 = acos(1/sqrt(3)), angle2 = acos(1/sqrt(2)), dist = 5 * sqrt(3);
        
        if (ImGui::CollapsingHeader("Camera configuration:"))
        {
            ImGui::Checkbox("Free viewpoint:", &freeViewpoint);
            ImGui::SameLine();
            ImGui::Checkbox("Orbit", &cameraOrbit);
            //if (cameraOrbit == true) cameraOrbit = false;
            if (cameraOrbit) {
                    freeViewpoint = false;
                    //printf("Orbiting\n");
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
        
        mycameraOrbit = cameraOrbit;
        static bool isScreenShot = false;
        //std::cout << "mycameraOrbit" << mycameraOrbit << std::endl;
        
        static char ExportmeshFilename[64] = "a.obj";
        static char screenShotFilename[64] = "a.bmp";
        render->updateExpObj(0, "dummy");
        if (ImGui::CollapsingHeader("Export to file:"))
        {
            freeViewpoint = false;
            ImGui::Text("Export mesh:");
            ImGui::InputText("File name###ExportmeshFilename", ExportmeshFilename, IM_ARRAYSIZE(ExportmeshFilename));
            ImGui::SameLine();
            if (ImGui::Button("export###mesh"))
            {
                render->updateExpObj(1, ExportmeshFilename);
            }

            ImGui::Text("Screen shot:");
            ImGui::InputText("File name###screenShotFilename", screenShotFilename, IM_ARRAYSIZE(screenShotFilename));
            ImGui::SameLine();
            if (ImGui::Button("export###img"))
            {
                    isScreenShot = true;
                    printf("Printing\n");
            }
            else {
                isScreenShot = false;
            }
        }
        tmpscreenShotFilename=screenShotFilename;
        re = tmpscreenShotFilename;
        if (freeViewpoint == 0) {
            re = "0" + re;
        }
        else {
            re = "1" + re;
        }
        if (isScreenShot == 0) {
            re = "0" + re;
        }
        else {
            re = "1" + re;
        }
        ImGui::End();
    }

    ImGui::Render();
    return re;
}
bool GUI:: isOrbit() {
    return mycameraOrbit;
}
bool GUI::isFreeViewpoint() {
    std::cout << "In function isfreeViewpoint" << myfreeViewpoint << std::endl;
    return myfreeViewpoint == 1;
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
