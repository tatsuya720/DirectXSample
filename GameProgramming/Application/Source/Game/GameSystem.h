#pragma once

class GameSystem
{
public:
    void Initialize();

    void Execute();

    static void CreateInstance()
    {
        DeleteInstance();
        s_instance = new GameSystem();
    }

    static void DeleteInstance()
    {
        if (s_instance == nullptr)
        {
            return;
        }

        delete s_instance;
        s_instance = nullptr;
    }

    static GameSystem& GetInstance()
    {
        return *s_instance;
    }

private:
    static inline GameSystem* s_instance;
    GameSystem() {}

};

// GameSystemの唯一のインスタンスを簡単に取得するためのマクロ
#define GAMESYS GameSystem::GetInstance()