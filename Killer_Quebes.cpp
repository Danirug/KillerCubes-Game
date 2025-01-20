// Killer_Quebes_G21285889.cpp: A program using the TL-Engine
//G21285899_Daniru Gihen 

#include <TL-Engine.h>// TL-Engine include file and namespace
#include <math.h>
using namespace tle;

enum GameState { // enum to represent the different states of the game
    ready, firing, contact, over
};

//struct to repseresent a block and avoid repition 
struct Block { 
    IModel* model; // pointer to the model of the block
    bool hit;//boolean to indicate if the bock has been hit 
};

// constants for game speed , rotation limit and others 
const float kGameSpeed = 1.2f;
const float RotationLimit = 50.0f;  
const int maximum_hits = 3;
const int cubes  = 12;

void main()
{
    // Create a 3D engine (using TLX engine here) and open a window for it
    I3DEngine* myEngine = New3DEngine(kTLX);
    myEngine->StartWindowed();

    // Add default folder for meshes and other media
    myEngine->AddMediaFolder(".\\Media");
    myEngine->AddMediaFolder("C:\\Users\\Daniru\\Desktop\\TL-engine projects\\Killer_Quebes_G21285889\\Media");

    /**** Set up your scene here ****/
    IMesh* floormesh = myEngine->LoadMesh("floor.x");
    IModel* floor = floormesh->CreateModel(0, -5, 0);

    IMesh* skyboxmesh = myEngine->LoadMesh("Skybox_Hell.x");
    IModel* skybox = skyboxmesh->CreateModel(0.0f, -1000.0f, 0.0f);

    IMesh* cubeMesh = myEngine->LoadMesh("Block.x");
    Block blocks[cubes];
    float startX = -60.0f; // starring X position for the blocks
    float gap = 12.0f;// gap between te blocks
    for (int i = 0; i < cubes  ; ++i)
    {
        blocks[i].model = cubeMesh->CreateModel(startX + i * gap, 0.0f, 120.0f);
        blocks[i].hit = false; // initialize the block as not hit 
    }


   /** IMesh* barrierMesh = myEngine->LoadMesh("Barrier.x");

    float xPositions[] = { -70.0f, 80.0f };  // Left and right side X positions

    for (int a = 0; a < 2; a++) {
        for (int i = 0; i < 10; i++) {
            float zPosition = i * 18.0f;
            IModel* barrier = barrierMesh->CreateModel(xPositions[a], 0, zPosition);

            if (i < 6) {
                barrier->SetSkin("barrier1a.bmp");  //  normal concrete skin for first 6 barriers
            }
            else {
                barrier->SetSkin("barrier1.bmp");   //  wasp stipe skin for last 4 barriers
            }
        }
    }*/


    IMesh* marblemesh = myEngine->LoadMesh("marble.x");
    IModel* marble = marblemesh->CreateModel(0, 0, 0);

    IMesh* arrowmesh = myEngine->LoadMesh("arrow.x");
    IModel* arrow = arrowmesh->CreateModel(0, 0, -10.0f);
    arrow->AttachToParent(marble);

   
    // Initialize game state varibles 
    GameState phase = ready;
    bool marbleMove = false;
    int blockCount = 0;
    float CurrentR = 0.0f;
    int hits = 0;
   

    ICamera* myCamera = myEngine->CreateCamera(kManual);
    myCamera->SetPosition(0, 30.0f, -60.0f);
    myCamera->RotateX(10.0f);

    // The main game loop, repeat until engine is stopped
    while (myEngine->IsRunning())
    {
        // Draw the scene
        myEngine->DrawScene();

        if (myEngine->KeyHit(Key_Escape)) // stops the game if the key escape is pressed
        {
            myEngine->Stop();
        }

        // game state handling 
        switch (phase)
        {
        case ready:
            if (myEngine->KeyHeld(Key_Z)) { // rotate the marble clockwise
                if (CurrentR + kGameSpeed <= RotationLimit) {
                    marble->RotateY(kGameSpeed);
                    CurrentR += kGameSpeed;
                }
            }

            if (myEngine->KeyHeld(Key_X)) { // rotate the marble counterclockwise
                if (CurrentR + kGameSpeed >= -RotationLimit) {
                    marble->RotateY(-kGameSpeed);
                    CurrentR -= kGameSpeed;
                }
            }

            if (myEngine->KeyHit(Key_Space)) { // launches the marble
                marbleMove = true;
                phase = firing;
            }

            break;

        case firing:
            if (marbleMove)
            {
                marble->MoveLocalZ(kGameSpeed); // moves marble forward

                // check for collisions with blocks
                for (int a = 0; a < 12; a++)
                {
                    float ab = marble->GetX() - blocks[a].model->GetX();// Calculate the difference in the X position
                    float bc = marble->GetZ() - blocks[a].model->GetZ();//Calculate the difference in the Z position
                    float distance = sqrt(ab * ab + bc * bc);// calculate the straight line distance between the marble druing the collison

                    if (distance < 10.0f) //collision detected 
                    {
                        if (!blocks[a].hit) // checks if block is not hit 
                        {
                            blocks[a].hit = true;
                            blocks[a].model->SetSkin("tiles_red.jpg");
                            hits++;
                            if (hits >= maximum_hits) // checks if maximum hits is reached 
                            {
                                marble ->SetSkin("glass_green.jpg");
                            }
                        }
                        marbleMove = false;
                        phase = contact; //  chnage the game state to contact 
                        break;
                    }

                }
                if (marble->GetZ() >= 200.0f) // checks if marble is out of bounds
                {
                    marbleMove = false;
                    phase = over;
                }
            }
            break;


         case contact: 
            if (hits >= maximum_hits) // checks if maximum hits is reached 
            {
                phase = over;
            }
            else
            {
                phase = over;
            }

            break;

        case over:
            if (myEngine->KeyHit(Key_R) && (hits < maximum_hits)) 
            {
                marble->SetLocalPosition(0.0f, 0.0f, 0.0f); // positions the marble to its default position 
                marble->SetSkin("glass.jpg");
                CurrentR = 0.0f;
                arrow->AttachToParent(marble);
                arrow->SetLocalPosition(0.0f, 0.0f, -10.0f);// positions the arrow to its default position
                marbleMove = false;
                phase = ready;
            }

            break;

        default:
            break;
        }

        /**** Update your scene each frame here ****/
    }

    // Delete the 3D engine now we are finished with it
    myEngine->Delete();
}
