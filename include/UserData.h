#pragma once

enum class UserDataType {
    PLAYER = 0,
    GROUND =1 ,
    BOSS=2,
    SKELETON =3,
    ITEM=4,


    // Otros tipos de UserData si los necesitas
};

struct UserData {
    UserDataType type;
    void* data;  // Información adicional si es necesario
};
