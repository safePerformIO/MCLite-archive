#include "MapGen.hpp"
#include "../Block/BlockList.hpp"
#include <cmath>
int randint(int min, int max) // min and max inclusive
{
    return (rand() % (max - min + 1)) + min;
}

Map MapGen::generate()
{
    chunkStream.resize(0);
    srand(seed);
    height = randint(40, 89);
    for (count = 0; count < 128;)
    {
        srand((seed + 1) * (count + 1));
        int _rand = randint(0, 4);

        // PS: The height limitationss are added to avoid array segementation fault when generating the map
        if (_rand == 0 && height < 54)
            chunkStream.push_back(generateTerr(3, true, HILL));
        else if (_rand == 1 && height > 74)
            chunkStream.push_back(generateTerr(3, false, HILL));
        else if (_rand == 2 && height < 44)
            chunkStream.push_back(generateTerr(6, true, CLIF));
        else if (_rand == 3 && height > 84)
            chunkStream.push_back(generateTerr(6, false, CLIF));
        else if (_rand == 4 && height > 65)
            chunkStream.push_back(cave());
        else
            chunkStream.push_back(generateTerr(3, true, GROUND));
    }

    std::array<Chunk::Ptr, 128> temp_internal;
    int i = 0;

    for (auto &chunk : chunkStream)
    {

        temp_internal.at(i++) = std::move(chunk);
    }

    Map map(std::move(temp_internal), nullptr);

    return map;
}

Chunk::Ptr MapGen::generateTerr(int modNumber, bool ascendFromL2R, TerrType terrType)
{
    std::unique_ptr<Chunk> temp(std::make_unique<Chunk>());

    // determine the number of trees within this chunk
    int numOfTreesInThisChunk = randint(0, 4);
    int treeInd1 = 0, treeInd2 = 0, treeInd3 = 0, treeInd4 = 0;
    switch (numOfTreesInThisChunk)
    {
    case 0:
        treeInd1 = treeInd2 = treeInd3 = treeInd4 = -5;
        break;
    case 1:
        treeInd1 = randint(1, 14);
        treeInd2 = treeInd3 = treeInd4 = -5;
        break;
    case 2:
        treeInd1 = randint(1, 6);
        treeInd2 = randint(9, 14);
        treeInd3 = treeInd4 = -5;
        break;
    case 3:
        treeInd1 = randint(1, 3);
        treeInd2 = randint(6, 8);
        treeInd3 = randint(11, 13);
        treeInd4 = -5;
        break;
    case 4:
        treeInd1 = randint(1, 2);
        treeInd2 = randint(5, 6);
        treeInd3 = randint(9, 10);
        treeInd4 = randint(13, 14);
        break;
    default:
        break;
    }

    srand(seed * count);

    // go through each column with in one chunk
    for (int i = 0; i < 16; i++)
    {
        // different terrains have different altitude variation
        int delta = 0;
        if (terrType == CLIF)
            delta = rand() % modNumber + 1;
        else if (terrType == GROUND)
            delta = rand() % modNumber - 1;
        else
            delta = rand() % modNumber;
        if (ascendFromL2R)
            height += delta;
        else
            height -= delta;

        // within one vertical column, different types of blocks should be generated in different portions of the terrain
        for (int j = 0; j < height; j++)
        {
            int choice = randint(0, 5); // 1/5 chance of getting one random number from 0-4

            if (j == 0) // unbreakable block at the very last row
                temp->internal.at(j)->at(i) = newBlock("bedrock");
            else if (j <= height / 3) // stones dominates the 1st 1/3 of the terrain
            {
                switch (choice)
                {
                case 0:
                    temp->internal.at(j)->at(i) = newBlock("coalore");
                    break;
                case 1:
                    temp->internal.at(j)->at(i) = newBlock("bedrock");
                    break;
                default:
                    temp->internal.at(j)->at(i) = newBlock("stone");
                    break;
                }
            }
            else if (j <= height / 2) // cobblestone dominates 1/2-1/3 of the terrain
            {
                switch (choice)
                {
                case 0:
                    temp->internal.at(j)->at(i) = newBlock("coalore");
                    break;
                case 1:
                    temp->internal.at(j)->at(i) = newBlock("stone");
                    break;
                default:
                    temp->internal.at(j)->at(i) = newBlock("cobblestone");
                    break;
                }
            }
            else if (j == height - 1) // the first row should be grass
            {
                temp->internal.at(j)->at(i) = newBlock("grass");
            }
            else // dirt for 1/2-top of the terrain
            {
                temp->internal.at(j)->at(i) = newBlock("dirt");
            }
        }

        if (i == treeInd1 || i == treeInd2 || i == treeInd3 || i == treeInd4) // generate the trees
        {
            for (int j = height; j < height + 3; j++)
            {
                temp->internal.at(j)->at(i) = newBlock("log");
            }
            for (int j = height + 3; j < height + 5; j++)
            {
                temp->internal.at(j)->at(i) = newBlock("leaves");
            }
            for (int j = height + 2; j < height + 5; j++)
            {
                if (temp->internal.at(j)->at(i - 1)->get() == Block::BlockType::AIR)
                    temp->internal.at(j)->at(i - 1) = newBlock("leaves");
                if (temp->internal.at(j)->at(i + 1)->get() == Block::BlockType::AIR)
                    temp->internal.at(j)->at(i + 1) = newBlock("leaves");
            }

            for (int j = height + 5; j < 128; j++)
            {
                temp->internal.at(j)->at(i) = newBlock("air");
            }
        }
        else
        {
            if (i != treeInd1 + 1 && i != treeInd2 + 1 && i != treeInd3 + 1 && i != treeInd4 + 1)
            {
                for (int j = height; j < 128; j++)
                {
                    temp->internal.at(j)->at(i) = newBlock("air");
                }
            }
        }
    }
    count++;

    return temp;
}

Chunk::Ptr MapGen::cave()
{
    static int caveWidth = randint(5, 13);
    static int caveStartInd = (16 - caveWidth) / 2;
    static int caveEndInd = caveStartInd + caveWidth - 1;

    std::unique_ptr<Chunk> temp(std::make_unique<Chunk>());

    srand(seed * count);
    int delta = 0, delta2 = 0;
    int height_1 = height - randint(18, 24);
    int height_2 = height_1 - randint(18, 35);

    for (int i = 0; i < 16; i++)
    {
        delta = randint(-2, 2);
        delta2 = randint(-1, 1);
        height += delta;
        height_1 += delta2;
        height_2 += delta2;

        if (i >= caveStartInd && i <= caveEndInd) // the cave is generated here
        {
            for (int j = 0; j < height_2; j++) // the solid terrain below the cave, same as normal terrain generation
            {
                int choice = randint(0, 5); // 1/5 chance of getting one random number from 0-4

                if (j == 0)
                    temp->internal.at(j)->at(i) = newBlock("bedrock");
                else if (j <= height / 3)
                {
                    switch (choice)
                    {
                    case 0:
                        temp->internal.at(j)->at(i) = newBlock("coalore");
                        break;
                    case 1:
                        temp->internal.at(j)->at(i) = newBlock("bedrock");
                        break;
                    default:
                        temp->internal.at(j)->at(i) = newBlock("stone");
                        break;
                    }
                }
                else if (j <= height / 2)
                {
                    switch (choice)
                    {
                    case 0:
                        temp->internal.at(j)->at(i) = newBlock("coalore");
                        break;
                    case 1:
                        temp->internal.at(j)->at(i) = newBlock("stone");
                        break;
                    default:
                        temp->internal.at(j)->at(i) = newBlock("cobblestone");
                        break;
                    }
                }
                else if (j == height - 1)
                {
                    temp->internal.at(j)->at(i) = newBlock("grass");
                }
                else
                {
                    temp->internal.at(j)->at(i) = newBlock("dirt");
                }
            }
            for (int j = height_2; j < height_1; j++) // This is the cave
            {
                temp->internal.at(j)->at(i) = newBlock("air");
            }
            for (int j = height_1; j < height; j++) // this is the terrain above the cave
            {
                int choice = randint(0, 5); // 1/5 chance of getting one random number from 0-4

                if (j == 0)
                    temp->internal.at(j)->at(i) = newBlock("bedrock");
                else if (j <= height / 3)
                {
                    switch (choice)
                    {
                    case 0:
                        temp->internal.at(j)->at(i) = newBlock("coalore");
                        break;
                    case 1:
                        temp->internal.at(j)->at(i) = newBlock("bedrock");
                        break;
                    default:
                        temp->internal.at(j)->at(i) = newBlock("stone");
                        break;
                    }
                }
                else if (j <= height / 2)
                {
                    switch (choice)
                    {
                    case 0:
                        temp->internal.at(j)->at(i) = newBlock("coalore");
                        break;
                    case 1:
                        temp->internal.at(j)->at(i) = newBlock("stone");
                        break;
                    default:
                        temp->internal.at(j)->at(i) = newBlock("cobblestone");
                        break;
                    }
                }
                else if (j == height - 1)
                {
                    temp->internal.at(j)->at(i) = newBlock("grass");
                }
                else
                {
                    temp->internal.at(j)->at(i) = newBlock("dirt");
                }
            }
            for (int j = height; j < 128; j++)
            {
                temp->internal.at(j)->at(i) = newBlock("air");
            }
        }
        else // this is same as the previous normal terrain generation
        {
            for (int j = 0; j < height; j++)
            {
                int choice = randint(0, 5); // 1/5 chance of getting one random number from 0-4

                if (j == 0)
                    temp->internal.at(j)->at(i) = newBlock("bedrock");
                else if (j <= height / 3)
                {
                    switch (choice)
                    {
                    case 0:
                        temp->internal.at(j)->at(i) = newBlock("coalore");
                        break;
                    case 1:
                        temp->internal.at(j)->at(i) = newBlock("bedrock");
                        break;
                    default:
                        temp->internal.at(j)->at(i) = newBlock("stone");
                        break;
                    }
                }
                else if (j <= height / 2)
                {
                    switch (choice)
                    {
                    case 0:
                        temp->internal.at(j)->at(i) = newBlock("coalore");
                        break;
                    case 1:
                        temp->internal.at(j)->at(i) = newBlock("stone");
                        break;
                    default:
                        temp->internal.at(j)->at(i) = newBlock("cobblestone");
                        break;
                    }
                }
                else if (j == height - 1)
                {
                    temp->internal.at(j)->at(i) = newBlock("grass");
                }
                else
                {
                    temp->internal.at(j)->at(i) = newBlock("dirt");
                }
            }
            for (int j = height; j < 128; j++)
            {
                temp->internal.at(j)->at(i) = newBlock("air");
            }
        }
    }
    count++;
    return temp;
}
