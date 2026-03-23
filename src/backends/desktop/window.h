//
// Created by Logrus on 22.03.2026.
//

#ifndef OGSENGINE_DESKTOP_WINDOW_H
#define OGSENGINE_DESKTOP_WINDOW_H

#include "backends/AppWindow.h"
#include "glfw3.h"
#include "input/KeyboardCodes.h"

namespace ogs {


class DesktopWindow : public AppWindow {
private:
    GLFWwindow* window = nullptr;
    GLFWmonitor* monitor = nullptr;
    const GLFWvidmode* vidmode = nullptr;
    Input* inputHandler = nullptr;
    float xscale = 1.0f, yscale = 1.0f;
    int xpos_work = 0, ypos_work = 0, wh_work = 0, ht_work = 0;
    int fbwidth = 0;
    int fbheight = 0;
    void* metalLayer = nullptr; // CAMetalLayer* on macOS, nullptr on other platforms

    void registerCallbacks();

public:
    DesktopWindow();
    ~DesktopWindow() override;
    void createWindow(int=-1, int=-1) override;
    void setInputHandler(Input* input) override;
    void pollEvents() override;
    bool shouldClose() const override;
    void* getNativeHandle() const override;
    void* getMetalLayer() const { return metalLayer; }

    std::unordered_map<int, KeyboardMap::KeyCode> g_KeyboardMap = {
            /* The unknown key */
            { GLFW_KEY_UNKNOWN         , KeyboardMap::KeyCode::KEY_NONE          },

            /* Printable keys */
            { GLFW_KEY_SPACE           , KeyboardMap::KeyCode::KEY_SPACE         },
            { GLFW_KEY_APOSTROPHE      , KeyboardMap::KeyCode::KEY_APOSTROPHE    },
            { GLFW_KEY_COMMA           , KeyboardMap::KeyCode::KEY_COMMA         },
            { GLFW_KEY_MINUS           , KeyboardMap::KeyCode::KEY_MINUS         },
            { GLFW_KEY_PERIOD          , KeyboardMap::KeyCode::KEY_PERIOD        },
            { GLFW_KEY_SLASH           , KeyboardMap::KeyCode::KEY_SLASH         },
            { GLFW_KEY_0               , KeyboardMap::KeyCode::KEY_0             },
            { GLFW_KEY_1               , KeyboardMap::KeyCode::KEY_1             },
            { GLFW_KEY_2               , KeyboardMap::KeyCode::KEY_2             },
            { GLFW_KEY_3               , KeyboardMap::KeyCode::KEY_3             },
            { GLFW_KEY_4               , KeyboardMap::KeyCode::KEY_4             },
            { GLFW_KEY_5               , KeyboardMap::KeyCode::KEY_5             },
            { GLFW_KEY_6               , KeyboardMap::KeyCode::KEY_6             },
            { GLFW_KEY_7               , KeyboardMap::KeyCode::KEY_7             },
            { GLFW_KEY_8               , KeyboardMap::KeyCode::KEY_8             },
            { GLFW_KEY_9               , KeyboardMap::KeyCode::KEY_9             },
            { GLFW_KEY_SEMICOLON       , KeyboardMap::KeyCode::KEY_SEMICOLON     },
            { GLFW_KEY_EQUAL           , KeyboardMap::KeyCode::KEY_EQUAL         },
            { GLFW_KEY_A               , KeyboardMap::KeyCode::KEY_A             },
            { GLFW_KEY_B               , KeyboardMap::KeyCode::KEY_B             },
            { GLFW_KEY_C               , KeyboardMap::KeyCode::KEY_C             },
            { GLFW_KEY_D               , KeyboardMap::KeyCode::KEY_D             },
            { GLFW_KEY_E               , KeyboardMap::KeyCode::KEY_E             },
            { GLFW_KEY_F               , KeyboardMap::KeyCode::KEY_F             },
            { GLFW_KEY_G               , KeyboardMap::KeyCode::KEY_G             },
            { GLFW_KEY_H               , KeyboardMap::KeyCode::KEY_H             },
            { GLFW_KEY_I               , KeyboardMap::KeyCode::KEY_I             },
            { GLFW_KEY_J               , KeyboardMap::KeyCode::KEY_J             },
            { GLFW_KEY_K               , KeyboardMap::KeyCode::KEY_K             },
            { GLFW_KEY_L               , KeyboardMap::KeyCode::KEY_L             },
            { GLFW_KEY_M               , KeyboardMap::KeyCode::KEY_M             },
            { GLFW_KEY_N               , KeyboardMap::KeyCode::KEY_N             },
            { GLFW_KEY_O               , KeyboardMap::KeyCode::KEY_O             },
            { GLFW_KEY_P               , KeyboardMap::KeyCode::KEY_P             },
            { GLFW_KEY_Q               , KeyboardMap::KeyCode::KEY_Q             },
            { GLFW_KEY_R               , KeyboardMap::KeyCode::KEY_R             },
            { GLFW_KEY_S               , KeyboardMap::KeyCode::KEY_S             },
            { GLFW_KEY_T               , KeyboardMap::KeyCode::KEY_T             },
            { GLFW_KEY_U               , KeyboardMap::KeyCode::KEY_U             },
            { GLFW_KEY_V               , KeyboardMap::KeyCode::KEY_V             },
            { GLFW_KEY_W               , KeyboardMap::KeyCode::KEY_W             },
            { GLFW_KEY_X               , KeyboardMap::KeyCode::KEY_X             },
            { GLFW_KEY_Y               , KeyboardMap::KeyCode::KEY_Y             },
            { GLFW_KEY_Z               , KeyboardMap::KeyCode::KEY_Z             },
            { GLFW_KEY_LEFT_BRACKET    , KeyboardMap::KeyCode::KEY_LEFT_BRACKET  },
            { GLFW_KEY_BACKSLASH       , KeyboardMap::KeyCode::KEY_BACK_SLASH    },
            { GLFW_KEY_RIGHT_BRACKET   , KeyboardMap::KeyCode::KEY_RIGHT_BRACKET },
            { GLFW_KEY_GRAVE_ACCENT    , KeyboardMap::KeyCode::KEY_GRAVE         },
            { GLFW_KEY_WORLD_1         , KeyboardMap::KeyCode::KEY_GRAVE         },
            { GLFW_KEY_WORLD_2         , KeyboardMap::KeyCode::KEY_NONE          },

            /* Function keys */
            { GLFW_KEY_ESCAPE          , KeyboardMap::KeyCode::KEY_ESCAPE        },
            { GLFW_KEY_ENTER           , KeyboardMap::KeyCode::KEY_ENTER      },
            { GLFW_KEY_TAB             , KeyboardMap::KeyCode::KEY_TAB           },
            { GLFW_KEY_BACKSPACE       , KeyboardMap::KeyCode::KEY_BACKSPACE     },
            { GLFW_KEY_INSERT          , KeyboardMap::KeyCode::KEY_INSERT        },
            { GLFW_KEY_DELETE          , KeyboardMap::KeyCode::KEY_DELETE        },
            { GLFW_KEY_RIGHT           , KeyboardMap::KeyCode::KEY_RIGHT_ARROW   },
            { GLFW_KEY_LEFT            , KeyboardMap::KeyCode::KEY_LEFT_ARROW    },
            { GLFW_KEY_DOWN            , KeyboardMap::KeyCode::KEY_DOWN_ARROW    },
            { GLFW_KEY_UP              , KeyboardMap::KeyCode::KEY_UP_ARROW      },
            { GLFW_KEY_PAGE_UP         , KeyboardMap::KeyCode::KEY_PG_UP      },
            { GLFW_KEY_PAGE_DOWN       , KeyboardMap::KeyCode::KEY_PG_DOWN    },
            { GLFW_KEY_HOME            , KeyboardMap::KeyCode::KEY_HOME       },
            { GLFW_KEY_END             , KeyboardMap::KeyCode::KEY_END           },
            { GLFW_KEY_CAPS_LOCK       , KeyboardMap::KeyCode::KEY_CAPS_LOCK     },
            { GLFW_KEY_SCROLL_LOCK     , KeyboardMap::KeyCode::KEY_SCROLL_LOCK   },
            { GLFW_KEY_NUM_LOCK        , KeyboardMap::KeyCode::KEY_NUM_LOCK      },
            { GLFW_KEY_PRINT_SCREEN    , KeyboardMap::KeyCode::KEY_PRINT         },
            { GLFW_KEY_PAUSE           , KeyboardMap::KeyCode::KEY_PAUSE         },
            { GLFW_KEY_F1              , KeyboardMap::KeyCode::KEY_F1            },
            { GLFW_KEY_F2              , KeyboardMap::KeyCode::KEY_F2            },
            { GLFW_KEY_F3              , KeyboardMap::KeyCode::KEY_F3            },
            { GLFW_KEY_F4              , KeyboardMap::KeyCode::KEY_F4            },
            { GLFW_KEY_F5              , KeyboardMap::KeyCode::KEY_F5            },
            { GLFW_KEY_F6              , KeyboardMap::KeyCode::KEY_F6            },
            { GLFW_KEY_F7              , KeyboardMap::KeyCode::KEY_F7            },
            { GLFW_KEY_F8              , KeyboardMap::KeyCode::KEY_F8            },
            { GLFW_KEY_F9              , KeyboardMap::KeyCode::KEY_F9            },
            { GLFW_KEY_F10             , KeyboardMap::KeyCode::KEY_F10           },
            { GLFW_KEY_F11             , KeyboardMap::KeyCode::KEY_F11           },
            { GLFW_KEY_F12             , KeyboardMap::KeyCode::KEY_F12           },
            { GLFW_KEY_F13             , KeyboardMap::KeyCode::KEY_NONE          },
            { GLFW_KEY_F14             , KeyboardMap::KeyCode::KEY_NONE          },
            { GLFW_KEY_F15             , KeyboardMap::KeyCode::KEY_NONE          },
            { GLFW_KEY_F16             , KeyboardMap::KeyCode::KEY_NONE          },
            { GLFW_KEY_F17             , KeyboardMap::KeyCode::KEY_NONE          },
            { GLFW_KEY_F18             , KeyboardMap::KeyCode::KEY_NONE          },
            { GLFW_KEY_F19             , KeyboardMap::KeyCode::KEY_NONE          },
            { GLFW_KEY_F20             , KeyboardMap::KeyCode::KEY_NONE          },
            { GLFW_KEY_F21             , KeyboardMap::KeyCode::KEY_NONE          },
            { GLFW_KEY_F22             , KeyboardMap::KeyCode::KEY_NONE          },
            { GLFW_KEY_F23             , KeyboardMap::KeyCode::KEY_NONE          },
            { GLFW_KEY_F24             , KeyboardMap::KeyCode::KEY_NONE          },
            { GLFW_KEY_F25             , KeyboardMap::KeyCode::KEY_NONE          },
            { GLFW_KEY_KP_0            , KeyboardMap::KeyCode::KEY_0             },
            { GLFW_KEY_KP_1            , KeyboardMap::KeyCode::KEY_1             },
            { GLFW_KEY_KP_2            , KeyboardMap::KeyCode::KEY_2             },
            { GLFW_KEY_KP_3            , KeyboardMap::KeyCode::KEY_3             },
            { GLFW_KEY_KP_4            , KeyboardMap::KeyCode::KEY_4             },
            { GLFW_KEY_KP_5            , KeyboardMap::KeyCode::KEY_5             },
            { GLFW_KEY_KP_6            , KeyboardMap::KeyCode::KEY_6             },
            { GLFW_KEY_KP_7            , KeyboardMap::KeyCode::KEY_7             },
            { GLFW_KEY_KP_8            , KeyboardMap::KeyCode::KEY_8             },
            { GLFW_KEY_KP_9            , KeyboardMap::KeyCode::KEY_9             },
            { GLFW_KEY_KP_DECIMAL      , KeyboardMap::KeyCode::KEY_PERIOD        },
            { GLFW_KEY_KP_DIVIDE       , KeyboardMap::KeyCode::KEY_KP_DIVIDE     },
            { GLFW_KEY_KP_MULTIPLY     , KeyboardMap::KeyCode::KEY_KP_MULTIPLY   },
            { GLFW_KEY_KP_SUBTRACT     , KeyboardMap::KeyCode::KEY_KP_MINUS      },
            { GLFW_KEY_KP_ADD          , KeyboardMap::KeyCode::KEY_KP_PLUS       },
            { GLFW_KEY_KP_ENTER        , KeyboardMap::KeyCode::KEY_KP_ENTER      },
            { GLFW_KEY_KP_EQUAL        , KeyboardMap::KeyCode::KEY_EQUAL         },
            { GLFW_KEY_LEFT_SHIFT      , KeyboardMap::KeyCode::KEY_LEFT_SHIFT         },
            { GLFW_KEY_LEFT_CONTROL    , KeyboardMap::KeyCode::KEY_LEFT_CTRL          },
            { GLFW_KEY_LEFT_ALT        , KeyboardMap::KeyCode::KEY_LEFT_ALT           },
            { GLFW_KEY_LEFT_SUPER      , KeyboardMap::KeyCode::KEY_HYPER         },
            { GLFW_KEY_RIGHT_SHIFT     , KeyboardMap::KeyCode::KEY_RIGHT_SHIFT         },
            { GLFW_KEY_RIGHT_CONTROL   , KeyboardMap::KeyCode::KEY_RIGHT_CTRL          },
            { GLFW_KEY_RIGHT_ALT       , KeyboardMap::KeyCode::KEY_RIGHT_ALT           },
            { GLFW_KEY_RIGHT_SUPER     , KeyboardMap::KeyCode::KEY_HYPER         },
            { GLFW_KEY_MENU            , KeyboardMap::KeyCode::KEY_MENU          },
            { GLFW_KEY_LAST            , KeyboardMap::KeyCode::KEY_NONE          }
    };
};

} // ogs

#endif //OGSENGINE_DESKTOP_WINDOW_H