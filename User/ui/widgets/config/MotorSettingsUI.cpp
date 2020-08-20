//
// Created by shubin on 02.06.2020.
//

#include "MotorSettingsUI.h"
#include "stepper.h"
#include "integration.h"
#include "mks_cfg.h"

#define fix_stepper(axis) {stepper.last_direction_bits = ((stepper.last_direction_bits & (1<<axis)) == 0) ? stepper.last_direction_bits | (1<<axis) : stepper.last_direction_bits & (~(1<<axis));}

#define E1_AXIS XYZE

MotorSettingsUI motor_settings_ui;

typedef enum {
    STEP_X, STEP_Y, STEP_Z, STEP_E0, STEP_E1,
    SPEED_X, SPEED_Y, SPEED_Z, SPEED_E0, SPEED_E1,
    JERK_X, JERK_Y, JERK_Z, JERK_E0,
    ACCEL_X,ACCEL_Y,ACCEL_Z,ACCEL_E0,ACCEL_E1,ACCEL_PRINT,ACCEL_RETRACT,ACCEL_TRAVEL,
};

void MotorSettingsUI::updateControls() {
    //steps
    if (this->ui.steps.x.button!=0) {
        sprintf(ui_buf1_100,"%.2f", Planner::axis_steps_per_mm[X_AXIS]);
        this->setButtonText(this->ui.steps.x.button, ui_buf1_100);
    }
    if (this->ui.steps.y.button!=0) {
        sprintf(ui_buf1_100,"%.2f",Planner::axis_steps_per_mm[Y_AXIS]);
        this->setButtonText(this->ui.steps.y.button, ui_buf1_100);
    }
    if (this->ui.steps.z.button!=0) {
        sprintf(ui_buf1_100,"%.2f",Planner::axis_steps_per_mm[Z_AXIS]);
        this->setButtonText(this->ui.steps.z.button, ui_buf1_100);
    }
    if (this->ui.steps.e0.button!=0) {
        sprintf(ui_buf1_100,"%.2f",Planner::axis_steps_per_mm[E_AXIS]);
        this->setButtonText(this->ui.steps.e0.button, ui_buf1_100);
    }
    if (this->ui.steps.e1.button!=0) {
        sprintf(ui_buf1_100,"%.2f",Planner::axis_steps_per_mm[E1_AXIS]);
        this->setButtonText(this->ui.steps.e1.button, ui_buf1_100);
    }

    //speed
    if (this->ui.speed.x.button!=0) {
        sprintf(ui_buf1_100,"%.1f", Planner::max_feedrate_mm_s[X_AXIS]);
        this->setButtonText(this->ui.speed.x.button, ui_buf1_100);
    }
    if (this->ui.speed.y.button!=0) {
        sprintf(ui_buf1_100,"%.1f",Planner::max_feedrate_mm_s[Y_AXIS]);
        this->setButtonText(this->ui.speed.y.button, ui_buf1_100);
    }
    if (this->ui.speed.z.button!=0) {
        sprintf(ui_buf1_100,"%.1f",Planner::max_feedrate_mm_s[Z_AXIS]);
        this->setButtonText(this->ui.speed.z.button, ui_buf1_100);
    }
    if (this->ui.speed.e0.button!=0) {
        sprintf(ui_buf1_100,"%.1f",Planner::max_feedrate_mm_s[E_AXIS]);
        this->setButtonText(this->ui.speed.e0.button, ui_buf1_100);
    }
    if (this->ui.speed.e1.button!=0) {
        sprintf(ui_buf1_100,"%.1f",Planner::max_feedrate_mm_s[E1_AXIS]);
        this->setButtonText(this->ui.speed.e1.button, ui_buf1_100);
    }

    //jerk
    //speed
    if (this->ui.jerk.x.button!=0) {
        sprintf(ui_buf1_100,"%.1f", Planner::max_jerk[X_AXIS]);
        this->setButtonText(this->ui.jerk.x.button, ui_buf1_100);
    }
    if (this->ui.jerk.y.button!=0) {
        sprintf(ui_buf1_100,"%.1f",Planner::max_jerk[Y_AXIS]);
        this->setButtonText(this->ui.jerk.y.button, ui_buf1_100);
    }
    if (this->ui.jerk.z.button!=0) {
        sprintf(ui_buf1_100,"%.1f",Planner::max_jerk[Z_AXIS]);
        this->setButtonText(this->ui.jerk.z.button, ui_buf1_100);
    }
    if (this->ui.jerk.e0.button!=0) {
        sprintf(ui_buf1_100,"%.1f",Planner::max_jerk[E_AXIS]);
        this->setButtonText(this->ui.jerk.e0.button, ui_buf1_100);
    }

    //accel
    if (this->ui.accel.x.button!=0) {
        sprintf(ui_buf1_100,"%d",(u32)Planner::max_acceleration_mm_per_s2[X_AXIS]);
        this->setButtonText(this->ui.accel.x.button, ui_buf1_100);
    }
    if (this->ui.accel.y.button!=0) {
        sprintf(ui_buf1_100,"%d",(u32)Planner::max_acceleration_mm_per_s2[Y_AXIS]);
        this->setButtonText(this->ui.accel.y.button, ui_buf1_100);
    }
    if (this->ui.accel.z.button!=0) {
        sprintf(ui_buf1_100,"%d",(u32)Planner::max_acceleration_mm_per_s2[Z_AXIS]);
        this->setButtonText(this->ui.accel.z.button, ui_buf1_100);
    }
    if (this->ui.accel.e0.button!=0) {
        sprintf(ui_buf1_100,"%d",(u32)Planner::max_acceleration_mm_per_s2[E_AXIS]);
        this->setButtonText(this->ui.accel.e0.button, ui_buf1_100);
    }
    if (this->ui.accel.e1.button!=0) {
        sprintf(ui_buf1_100,"%d",(u32)Planner::max_acceleration_mm_per_s2[E1_AXIS]);
        this->setButtonText(this->ui.accel.e1.button, ui_buf1_100);
    }
    if (this->ui.accel.print.button!=0) {
        sprintf(ui_buf1_100,"%.0f",Planner::acceleration);
        this->setButtonText(this->ui.accel.print.button, ui_buf1_100);
    }
    if (this->ui.accel.travel.button!=0) {
        sprintf(ui_buf1_100,"%.0f",Planner::travel_acceleration);
        this->setButtonText(this->ui.accel.travel.button, ui_buf1_100);
    }
    if (this->ui.accel.retract.button!=0) {
        sprintf(ui_buf1_100,"%.0f",Planner::retract_acceleration);
        this->setButtonText(this->ui.accel.retract.button, ui_buf1_100);
    }
}


void MotorSettingsUI::_setValue(unsigned char index, float value) {
    switch(index) {
        default:
            break;
        case STEP_X: {
            Planner::axis_steps_per_mm[X_AXIS] = value;
            break;
        }
        case STEP_Y: {
            Planner::axis_steps_per_mm[Y_AXIS] = value;
            break;
        }
        case STEP_Z: {
            Planner::axis_steps_per_mm[Z_AXIS] = value;
            break;
        }
        case STEP_E0: {
            Planner::axis_steps_per_mm[E_AXIS] = value;
            break;
        }
        case STEP_E1: {
            Planner::axis_steps_per_mm[E1_AXIS] = value;
            break;
        }

        case SPEED_X: {
            Planner::max_feedrate_mm_s[X_AXIS] = value;
            break;
        }
        case SPEED_Y: {
            Planner::max_feedrate_mm_s[Y_AXIS] = value;
            break;
        }
        case SPEED_Z: {
            Planner::max_feedrate_mm_s[Z_AXIS] = value;
            break;
        }
        case SPEED_E0: {
            Planner::max_feedrate_mm_s[E_AXIS] = value;
            break;
        }
        case SPEED_E1: {
            Planner::max_feedrate_mm_s[E1_AXIS] = value;
            break;
        }


        case JERK_X: {
            Planner::max_jerk[X_AXIS] = value;
            break;
        }
        case JERK_Y: {
            Planner::max_jerk[Y_AXIS] = value;
            break;
        }
        case JERK_Z: {
            Planner::max_jerk[Z_AXIS] = value;
            break;
        }
        case JERK_E0: {
            Planner::max_jerk[E_AXIS] = value;
            break;
        }

        case ACCEL_X: {
            Planner::max_acceleration_mm_per_s2[X_AXIS] = (unsigned long)value;
            break;
        }
        case ACCEL_Y: {
            Planner::max_acceleration_mm_per_s2[Y_AXIS] = (unsigned long)value;
            break;
        }
        case ACCEL_Z: {
            Planner::max_acceleration_mm_per_s2[Z_AXIS] = (unsigned long)value;
            break;
        }
        case ACCEL_E0: {
            Planner::max_acceleration_mm_per_s2[E_AXIS] = (unsigned long)value;
            break;
        }
        case ACCEL_E1: {
            Planner::max_acceleration_mm_per_s2[E1_AXIS] = (unsigned long)value;
            break;
        }
        case ACCEL_PRINT: {
            Planner::acceleration = value;
            break;
        }
        case ACCEL_TRAVEL: {
            Planner::travel_acceleration = value;
            break;
        }
        case ACCEL_RETRACT: {
            Planner::retract_acceleration = value;
            break;
        }
    }
    shUI::saveConfig();

}

void MotorSettingsUI::on_button(UI_BUTTON hBtn) {
    //// axis direction
    if (hBtn == this->ui.dir.x.button) {
        mksCfg.invert_x_dir = mksCfg.invert_x_dir != 0 ? 0 : 1;
        epr_write_data(EPR_INVERT_X_DIR, &mksCfg.invert_x_dir, 1);
        fix_stepper(X_AXIS);
        this->updateCheckButton(this->ui.dir.x.button, mksCfg.invert_x_dir);
    } else if (hBtn == this->ui.dir.y.button) {
        mksCfg.invert_y_dir = mksCfg.invert_y_dir != 0 ? 0 : 1;
        epr_write_data(EPR_INVERT_Y_DIR, &mksCfg.invert_y_dir, 1);
        fix_stepper(Y_AXIS);
        this->updateCheckButton(this->ui.dir.y.button, mksCfg.invert_y_dir);
    } else if (hBtn == this->ui.dir.z.button) {
        mksCfg.invert_z_dir = mksCfg.invert_z_dir != 0 ? 0 : 1;
        epr_write_data(EPR_INVERT_Z_DIR, &mksCfg.invert_z_dir, 1);
        fix_stepper(Z_AXIS);
        this->updateCheckButton(this->ui.dir.z.button, mksCfg.invert_z_dir);
    } else if (hBtn == this->ui.dir.e0.button) {
        mksCfg.invert_e0_dir = mksCfg.invert_e0_dir != 0 ? 0 : 1;
        epr_write_data(EPR_INVERT_E0_DIR, &mksCfg.invert_e0_dir, 1);
        fix_stepper(E_AXIS);
        this->updateCheckButton(this->ui.dir.e0.button, mksCfg.invert_e0_dir);
    } else if (hBtn == this->ui.dir.e1.button) {
        mksCfg.invert_e1_dir = mksCfg.invert_e1_dir != 0 ? 0 : 1;
        epr_write_data(EPR_INVERT_E1_DIR, &mksCfg.invert_e1_dir, 1);
        fix_stepper(E_AXIS);
        this->updateCheckButton(this->ui.dir.e1.button, mksCfg.invert_e1_dir);

    //steps
    } else if (hBtn == this->ui.steps.x.button) {
        this->calculator(lang_str.config_ui.steps, "/X", Planner::axis_steps_per_mm[X_AXIS], STEP_X);
    } else if (hBtn == this->ui.steps.y.button) {
        this->calculator(lang_str.config_ui.steps, "/Y", Planner::axis_steps_per_mm[Y_AXIS], STEP_Y);
    } else if (hBtn == this->ui.steps.z.button) {
        this->calculator(lang_str.config_ui.steps, "/Z", Planner::axis_steps_per_mm[Z_AXIS], STEP_Z);
    } else if (hBtn == this->ui.steps.e0.button) {
        this->calculator(lang_str.config_ui.steps, "/E0", Planner::axis_steps_per_mm[E_AXIS], STEP_E0);
    } else if (hBtn == this->ui.steps.e1.button) {
        this->calculator(lang_str.config_ui.steps, "/E1", Planner::axis_steps_per_mm[E1_AXIS], STEP_E1);

        //speeed
    } else if (hBtn == this->ui.speed.x.button) {
        this->calculator(lang_str.config_ui.maximum_speed, "/X", Planner::max_feedrate_mm_s[X_AXIS], SPEED_X);
    } else if (hBtn == this->ui.speed.y.button) {
        this->calculator(lang_str.config_ui.maximum_speed, "/Y", Planner::max_feedrate_mm_s[Y_AXIS], SPEED_Y);
    } else if (hBtn == this->ui.speed.z.button) {
        this->calculator(lang_str.config_ui.maximum_speed, "/Z", Planner::max_feedrate_mm_s[Z_AXIS], SPEED_Z);
    } else if (hBtn == this->ui.speed.e0.button) {
        this->calculator(lang_str.config_ui.maximum_speed, "/E0", Planner::max_feedrate_mm_s[E_AXIS], SPEED_E0);
    } else if (hBtn == this->ui.speed.e1.button) {
        this->calculator(lang_str.config_ui.maximum_speed, "/E1", Planner::max_feedrate_mm_s[E1_AXIS], SPEED_E1);

        //jerk
    } else if (hBtn == this->ui.jerk.x.button) {
        this->calculator(lang_str.config_ui.maximum_speed, "/X", Planner::max_jerk[X_AXIS], JERK_X);
    } else if (hBtn == this->ui.jerk.y.button) {
        this->calculator(lang_str.config_ui.maximum_speed, "/Y", Planner::max_jerk[Y_AXIS], JERK_Y);
    } else if (hBtn == this->ui.jerk.z.button) {
        this->calculator(lang_str.config_ui.maximum_speed, "/Z", Planner::max_jerk[Z_AXIS], JERK_Z);
    } else if (hBtn == this->ui.jerk.e0.button) {
        this->calculator(lang_str.config_ui.maximum_speed, "/E", Planner::max_jerk[E_AXIS], JERK_E0);

        //acceleration
    } else if (hBtn == this->ui.accel.x.button) {
        this->calculator(lang_str.config_ui.acceleration, "/X", (double)Planner::max_acceleration_mm_per_s2[X_AXIS], ACCEL_X);
    } else if (hBtn == this->ui.accel.y.button) {
        this->calculator(lang_str.config_ui.acceleration, "/Y", (double)Planner::max_acceleration_mm_per_s2[Y_AXIS], ACCEL_Y);
    } else if (hBtn == this->ui.accel.z.button) {
        this->calculator(lang_str.config_ui.acceleration, "/Z", (double)Planner::max_acceleration_mm_per_s2[Z_AXIS], ACCEL_Z);
    } else if (hBtn == this->ui.accel.e0.button) {
        this->calculator(lang_str.config_ui.acceleration, "/E0", (double)Planner::max_acceleration_mm_per_s2[E_AXIS], ACCEL_E0);
    } else if (hBtn == this->ui.accel.e1.button) {
        this->calculator(lang_str.config_ui.acceleration, "/E1", (double)Planner::max_acceleration_mm_per_s2[E1_AXIS], ACCEL_E1);
    } else if (hBtn == this->ui.accel.print.button) {
        strcpy(ui_buf1_100, lang_str.config_ui.acceleration);
        strcat(ui_buf1_100, "/");
        strcat(ui_buf1_100, lang_str.config_ui.print);
        this->calculator(ui_buf1_100, Planner::acceleration, ACCEL_PRINT);
    } else if (hBtn == this->ui.accel.travel.button) {
        strcpy(ui_buf1_100, lang_str.config_ui.acceleration);
        strcat(ui_buf1_100, "/");
        strcat(ui_buf1_100, lang_str.config_ui.travel);
        this->calculator(ui_buf1_100, Planner::travel_acceleration, ACCEL_TRAVEL);
    } else if (hBtn == this->ui.accel.retract.button) {
        strcpy(ui_buf1_100, lang_str.config_ui.acceleration);
        strcat(ui_buf1_100, "/");
        strcat(ui_buf1_100, lang_str.config_ui.retract);
        this->calculator(ui_buf1_100, Planner::retract_acceleration, ACCEL_RETRACT);
        //defaults
    } else {
        //steps
        if (hBtn == this->ui.steps.x.dflt) {
            this->_setValue(STEP_X, 80);
        } else if (hBtn == this->ui.steps.y.dflt) {
            this->_setValue(STEP_Y, 80);
        } else if (hBtn == this->ui.steps.z.dflt) {
            this->_setValue(STEP_Z, 400);
        } else if (hBtn == this->ui.steps.e0.dflt) {
            this->_setValue(STEP_E0, 90);
        } else if (hBtn == this->ui.steps.e1.dflt) {
            this->_setValue(STEP_E1, 90);

            //speeed
        } else if (hBtn == this->ui.speed.x.dflt) {
            this->_setValue(SPEED_X, 200);
        } else if (hBtn == this->ui.speed.y.dflt) {
            this->_setValue(SPEED_Y, 200);
        } else if (hBtn == this->ui.speed.z.dflt) {
            this->_setValue(SPEED_Z, 4);
        } else if (hBtn == this->ui.speed.e0.dflt) {
            this->_setValue(SPEED_E0, 70);
        } else if (hBtn == this->ui.speed.e1.dflt) {
            this->_setValue(SPEED_E1, 70);


            //jerk
        } else if (hBtn == this->ui.jerk.x.dflt) {
            this->_setValue(JERK_X, 10);
        } else if (hBtn == this->ui.jerk.y.dflt) {
            this->_setValue(JERK_Y, 10);
        } else if (hBtn == this->ui.jerk.z.dflt) {
            this->_setValue(JERK_Z, 0.3);
        } else if (hBtn == this->ui.jerk.e0.dflt) {
            this->_setValue(JERK_E0, 5);

            //acceleration
        } else if (hBtn == this->ui.accel.x.dflt) {
            this->_setValue(ACCEL_X, 1000);
        } else if (hBtn == this->ui.accel.y.dflt) {
            this->_setValue(ACCEL_Y, 1000);
        } else if (hBtn == this->ui.accel.z.dflt) {
            this->_setValue(ACCEL_Z, 100);
        } else if (hBtn == this->ui.accel.e0.dflt) {
            this->_setValue(ACCEL_E0, 1000);
        } else if (hBtn == this->ui.accel.e1.dflt) {
            this->_setValue(ACCEL_E1, 1000);
        } else if (hBtn == this->ui.accel.print.dflt) {
            this->_setValue(ACCEL_PRINT, 1000);
        } else if (hBtn == this->ui.accel.travel.dflt) {
            this->_setValue(ACCEL_TRAVEL, 1000);
        } else if (hBtn == this->ui.accel.retract.dflt) {
            this->_setValue(ACCEL_RETRACT, 1000);
        } else {
            ConfigurationWidget::on_button(hBtn);
            return;
        }
        this->updateControls();
    }
}

void MotorSettingsUI::createControls() {
    ConfigurationWidget::createControls();
    memset(&this->ui, 0, sizeof(this->ui));
    this->dual_columns = 1;
    switch (page) {
        case 0: {
            this->createCheckPair(0, 0, &this->ui.dir.x, "X:", mksCfg.invert_x_dir);
            this->createCheckPair(0, 1, &this->ui.dir.y, "Y:", mksCfg.invert_y_dir);
            this->createCheckPair(0, 2, &this->ui.dir.z, "Z:", mksCfg.invert_z_dir);
            this->createCheckPair(1, 0, &this->ui.dir.e0, "E0:", mksCfg.invert_e0_dir);
            this->createCheckPair(1, 1, &this->ui.dir.e1, "E1:", mksCfg.invert_e1_dir);
            break;
        }
        case 1: {
            this->createInputWithDefault(0, 0, &this->ui.steps.x, "X:", 0);
            this->createInputWithDefault(0, 1, &this->ui.steps.y, "Y:", 0);
            this->createInputWithDefault(0, 2, &this->ui.steps.z, "Z:", 0);
            this->createInputWithDefault(1, 0, &this->ui.steps.e0, "E0:", 0);
            this->createInputWithDefault(1, 1, &this->ui.steps.e1, "E1:", 0);
            break;
        }
        case 2: {
            this->createInputWithDefault(0, 0, &this->ui.accel.x, "X:", 0);
            this->createInputWithDefault(0, 1, &this->ui.accel.y, "Y:", 0);
            this->createInputWithDefault(0, 2, &this->ui.accel.z, "Z:", 0);
            this->createInputWithDefault(1, 0, &this->ui.accel.e0, "E0:", 0);
            this->createInputWithDefault(1, 1, &this->ui.accel.e1, "E1:", 0);

            this->createInputWithDefault(0, 3, &this->ui.accel.print, lang_str.config_ui.print, 0);
            this->createInputWithDefault(1, 2, &this->ui.accel.travel, lang_str.config_ui.travel, 0);
            this->createInputWithDefault(1, 3, &this->ui.accel.retract, lang_str.config_ui.retract, 0);
            break;
        }
        case 3: {
            this->createInputWithDefault(0, 0, &this->ui.speed.x, "X:", 0);
            this->createInputWithDefault(0, 1, &this->ui.speed.y, "Y:", 0);
            this->createInputWithDefault(0, 2, &this->ui.speed.z, "Z:", 0);
            this->createInputWithDefault(1, 0, &this->ui.speed.e0, "E0:", 0);
            this->createInputWithDefault(1, 1, &this->ui.speed.e1, "E1:", 0);
            break;
        }
        case 4: {
            this->createInputWithDefault(0, 0, &this->ui.jerk.x, "X:", 0);
            this->createInputWithDefault(0, 1, &this->ui.jerk.y, "Y:", 0);
            this->createInputWithDefault(0, 2, &this->ui.jerk.z, "Z:", 0);
            this->createInputWithDefault(1, 0, &this->ui.jerk.e0, "E:", 0);
            break;
        }
    }
    this->updateControls();
}
