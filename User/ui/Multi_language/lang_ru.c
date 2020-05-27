#include "lang_common.h"

void lang_ru(void) {
	lang_str.config_ui.power_off_after_print = "Выключить по окончании печати";
	lang_str.config_ui.simple_main_ui = "Простое меню";
	lang_str.config_ui.display_backlight_off = "Выключать подсветку дисплея";
	lang_str.config_ui.axes_inversion = "Инверсия осей";
	lang_str.config_ui.motor_steps_settings = "Настройка шагов";
	lang_str.config_ui.home_speed_settings = "Скорость парковки";
	lang_str.config_ui.acceleration_settings = "Настройки ускорений";
	lang_str.config_ui.maximum_speed_settings = "Максимальные скорости";
	lang_str.config_ui.jerk_settings = "Настройки рывков";

    lang_str.config_ui.machine_size = "Размеры";
    lang_str.config_ui.machine_type = "Тип механики";
    lang_str.config_ui.home_direction = "Парковка";
    lang_str.config_ui.endtop_type = "Датчики";
    lang_str.config_ui.filament_settings = "Филамент";
    lang_str.config_ui.manual_leveling_settings = "Уровни";
    lang_str.config_ui.auto_leveling_settings = "Автоуровени";
    lang_str.config_ui.nozzle_settings = "Сопло";
    lang_str.config_ui.hotbed_settings = "Стол";
    lang_str.config_ui.filament_change_speed = "Скорость:";
    lang_str.config_ui.filament_change_length= "Длина:";;

    lang_str.dialog.refactored_by = "Доработка ПО:\nВячеслав Шубин\nshubin-vv@krista.ru";

	lang_str.preheat = "Нагрев";
	lang_str.move = "Перемещение";
	lang_str.home = "Парковка";
	lang_str.leveling = "Уровень";
	lang_str.filament = "Филамент";
	lang_str.back = "Назад";
	lang_str.more = "Больше";
	lang_str.print = "Печать";
	lang_str.settings = "Настройки";
	lang_str.tools = "Инструменты";

	lang_str.plus = "Добавить";
	lang_str.minus = "Уменьшить";

	lang_str.off = "Выключить";
	lang_str.bed = "Стол";
	lang_str.extruder1 = "Экструдер 1";
	lang_str.extruder2 =  "Экструдер 2";

	lang_str.stop = "Стоп";

	lang_str.load = "Загрузить";
	lang_str.unload = "Вытянуть";
	lang_str.change = "Замена";

	lang_str.fan = "Вентилятор";
	lang_str.speed = "Скорость";
	lang_str.babysteps = "Микрошаги";
	lang_str.continue_print = "Продолжить";

	lang_str.machine = "Принтер";
	lang_str.temperature = "Нагрев";
	lang_str.motors = "Моторы";
	lang_str.advanced = "Дополнительно";

	lang_str.yes = "Да";
	lang_str.no = "Нет";
	lang_str.machine_settings = "Конфигурация";
	lang_str.language = "Язык";
	lang_str.about = "Версия";

	lang_str.overflow = "ПЕРЕПОЛНЕНИЕ";

    lang_str.ui_title_main = "Готов";
	lang_str.ui_title_tools = "Выбор инструмента";
	lang_str.ui_title_file_browser = "Выбор файла";
	lang_str.ui_title_babysteps = "Настройка сдвига";
	lang_str.ui_title_settings = "Выбор настроек";
	lang_str.ui_title_language = "Выбор языка";
	lang_str.ui_title_preheat = "Управление нагревателями";
	lang_str.ui_title_fan = "Управление вентилятором";
	lang_str.ui_title_filament = "Управление филаментом";
	lang_str.ui_title_move = "Управление моторами";
	lang_str.ui_title_home = "Установка нолей осей";
	lang_str.ui_title_leveling = "Настройка уровня стола";
	lang_str.ui_title_config = "Конфигурирование";
	lang_str.ui_title_more = "Дополнительно...";
	lang_str.ui_title_printing_tools = "Подстройка печати";
	lang_str.ui_title_printing= "Печать";
	lang_str.ui_title_speed = "Подстройка скорости";
    lang_str.ui_title_config_machine_size = "Размеры области печати";
    lang_str.ui_title_config_filament_change = "Настройки замены филамента";

	lang_str.file_info_ui.filament = "Филамент (m): %1.3f m";
    lang_str.file_info_ui.filament_progress = "Филамент (m): %1.3f / %1.3f %d%%";
	lang_str.file_info_ui.size = "Размер файла: %d";
    lang_str.file_info_ui.size_progress = "Чтение файла: %d / %d %d%%";
	lang_str.file_info_ui.time = "Время печати: %s";
    lang_str.file_info_ui.time_progress = "Время печати: %s / %s %d%%";
	lang_str.file_info_ui.layers = "Слои: %1.2f * %d";
    lang_str.file_info_ui.layers_progress = "Слои: %1.2f * %d / %d %d%%";

	lang_str.dialog.confirm_delete_file = "Удалить файл:\n%s";
	lang_str.dialog.confirm_terminate_print = "Прервать печать?";
	lang_str.dialog.confirm_print_again = "Повторить печать?";
	lang_str.dialog.confirm_print_with_suicide = "Автоматическое выключение.\nПрервать и повторить печать?";
	lang_str.dialog.error_filament_end_detected = "Обнаружен обрыв филамента";

}
