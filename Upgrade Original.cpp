#include <gtkmm.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include <thread>
#include <filesystem>
#include <unordered_map>
#include <algorithm>
#include <stdexcept>

// 1. Фишки иногда перескакивают назад/на старт
// 2. Хз как обновлять label после каждого хода
// 3. Добавить окно и механику прокачки улиц
// 4. Выводить выпавшие очки и следующего игрока
// 5. ВОЗМОЖНО: сделать "плавные ходы", перемещая фишку на 1 клетку вперёд, пока она не достигнет нужной 
// 6. Сделать инфу об игроках

// RGB
class RGB
    {
    public:
        double R = 0;
        double G = 0;
        double B = 0;
        RGB(){}
        RGB(int R_, int G_, int B_){R = R_; G = G_; B = B_;}
    };

    // Фишки
class Chip : public RGB
    {
    public:
        int x = 0;
        int y = 0;
        int money = 3000;
        int pos = 0;
        int A_Way_Out = 0;
        char col;
        int Jail = 0;
        std::string name = "";  // Имя игрока
        Chip(){}
        Chip(int x_, int y_, int R, int G, int B, char col_) : RGB(R, G, B){
            x = x_;
            y = y_;
            col = col_;
        }
    };

class Cell {
public:
    int x;
    int y;
    int count_of_pl = 0;
    std::vector<Cell> slots;

    // Конструктор по умолчанию
    Cell() : x(0), y(0) {}  // Всегда инициализируйте поля!

    // Основной конструктор (БЕЗ автоматического создания слотов!)
    Cell(int x_, int y_) : x(x_), y(y_) {}

    // Конструктор с предустановленными слотами (без рекурсии!)
    Cell(int x_, int y_, std::vector<Cell> slots_) 
        : x(x_), y(y_), slots(slots_) {}
};

// Вспомогательная функция для создания слотов (без рекурсии!)
std::vector<Cell> create_slots(int center_x, int center_y) {
    return {
        Cell(center_x - 22, center_y - 27),
        Cell(center_x + 22, center_y - 27),
        Cell(center_x - 22, center_y + 27),
        Cell(center_x + 22, center_y + 27)
    };
}

std::vector<Cell> left_down_cell = {
    Cell(670, 1160), 
    Cell(670, 1230), 
    Cell(790, 1282), 
    Cell(720, 1282)
};
std::vector<Cell> left_up_cell = {//733 172
    Cell(699, 89), 
    Cell(769, 159), 
    Cell(699, 159), 
    Cell(769, 89)
};
std::vector<Cell> right_up_cell = {
    Cell(1791, 89), 
    Cell(1861, 159), 
    Cell(1791, 159), 
    Cell(1861, 89)
};
std::vector<Cell> right_down_cell = {
    Cell(1807, 1196), 
    Cell(1877, 1266), 
    Cell(1807, 1266), 
    Cell(1877, 1196)
};

std::vector<Cell> Cells = {
    Cell(0, 0, right_down_cell),
    Cell(1687, 1235, create_slots(1687, 1235)), Cell(1585, 1235, create_slots(1585, 1235)), Cell(1483, 1235, create_slots(1483, 1235)), Cell(1381, 1235, create_slots(1381, 1235)), Cell(1279, 1235, create_slots(1279, 1235)), Cell(1177, 1235, create_slots(1177, 1235)), Cell(1075, 1235, create_slots(1075, 1235)), Cell(973, 1235, create_slots(973, 1235)), Cell(871, 1235, create_slots(871, 1235)), 
    Cell(0, 0, left_down_cell),  
    Cell(715, 1075, create_slots(715, 1075)),  Cell(715, 974, create_slots(715, 974)), Cell(715, 873, create_slots(715, 873)), Cell(715, 772, create_slots(715, 772)), Cell(715, 671, create_slots(715, 671)), Cell(715, 570, create_slots(715, 570)), Cell(715, 469, create_slots(715, 469)), Cell(715, 368, create_slots(715, 368)), Cell(715, 267, create_slots(715, 267)),
    Cell(0, 0, left_up_cell),
    Cell(871, 106, create_slots(871, 106)), Cell(973, 106, create_slots(973, 106)), Cell(1075, 106, create_slots(1075, 106)), Cell(1177, 106, create_slots(1177, 106)), Cell(1279, 106, create_slots(1279, 106)), Cell(1381, 106, create_slots(1381, 106)), Cell(1483, 106, create_slots(1483, 106)), Cell(1585, 106, create_slots(1585, 106)), Cell(1687, 106, create_slots(1687, 106)),
    Cell(0, 0, right_up_cell),
    Cell(1844, 267, create_slots(1844, 267)), Cell(1844, 368, create_slots(1844, 368)), Cell(1844, 469, create_slots(1844, 469)), Cell(1844, 570, create_slots(1844, 570)), Cell(1844, 671, create_slots(1844, 671)), Cell(1844, 772, create_slots(1844, 772)), Cell(1844, 873, create_slots(1844, 873)), Cell(1844, 974, create_slots(1844, 974)), Cell(1844, 1075, create_slots(1844, 1075))
};


int cur_player = 0;
int bounce = 0;
std::vector <Chip>Chips;
//street
class Chance
{
    public:
        std::string Chance_path;
        int money_for_pay = 0;
        int pos_to_move = -1;

        Chance(std::string Chance_path_, int money_for_pay_, int pos_to_move_){
            Chance_path = Chance_path_;
            money_for_pay = money_for_pay_;
            pos_to_move = pos_to_move_;
        }



};
class Street
{
public:
    int x_beg;
    int x_end;
    int y_beg;
    int y_end;
    int cost;
    int level = 0;
    int owner = -1;
    int cost_for_lvl_up;
    int X_for_sq;
    int Y_for_sq;
    std::string Path;
    std::string colour;
    std::string name;
    std::vector <int> rent;
    std::string type = "street";
    bool monopoly_bonus_applied = false;

    Street(int x_beg_, int x_end_, int y_beg_, int y_end_, std::string Path_, std::string colour_, int cost_, int cost_for_lvl_up_, int X_for_sq_, int Y_for_sq_, std::vector <int> rent_ = std::vector <int> ()){
        x_beg = x_beg_;
        x_end = x_end_;
        y_beg = y_beg_;
        y_end = y_end_;
        Path = Path_;
        colour = colour_;
        try {
            std::filesystem::path p(Path);
            name = p.stem().string();
        } catch(...) {
            name = Path;
        }
        cost = cost_;
        cost_for_lvl_up = cost_for_lvl_up_;
        X_for_sq = X_for_sq_;
        Y_for_sq = Y_for_sq_;
        rent = rent_;
    }

    std::pair <int, int> coord_for_lvl(){
        return std::make_pair(X_for_sq + 9, Y_for_sq + 22);
    }

    bool Check_str(int x, int y){
        if(x <= x_end && x >= x_beg && y <= y_end && y >= y_beg)
            return true;
        else
            return false;
    }
};


class Drawing_panel : public Gtk::DrawingArea
    {
    private:
        Glib::RefPtr <Gdk::Pixbuf> Area;
        Glib::RefPtr <Gdk::Pixbuf> Red_Credit_Card;
        Glib::RefPtr <Gdk::Pixbuf> Green_Credit_Card;
        Glib::RefPtr <Gdk::Pixbuf> Blue_Credit_Card;
        Glib::RefPtr <Gdk::Pixbuf> Yellow_Credit_Card;
        Glib::RefPtr <Gdk::Pixbuf> chance;

        Glib::RefPtr <Gdk::Pixbuf> Card_img;
        bool space_pressed = false;
        bool animation_in_progress = false;  // Флаг для блокировки ввода во время анимации
        std::random_device rd;
        std::unordered_map<std::string, Glib::RefPtr<Gdk::Pixbuf>> card_pixbuf_cache;
        int displayed_turn_player_index = -1; // Индекс игрока для показа в блоке "текущий/следующий ход"
        int highlighted_cell_index = -1;      // Индекс целевой клетки после броска
        Glib::RefPtr<Gdk::Pixbuf> load_pixbuf_scaled_cached(const std::string& path, int w, int h) {
            auto it = card_pixbuf_cache.find(path);
            if (it != card_pixbuf_cache.end()) return it->second;
            if (!Glib::file_test(path, Glib::FILE_TEST_EXISTS)) return Glib::RefPtr<Gdk::Pixbuf>();
            auto pix = Gdk::Pixbuf::create_from_file(path);
            pix = pix->scale_simple(w, h, Gdk::INTERP_BILINEAR);
            card_pixbuf_cache[path] = pix;
            return pix;
        }
        std::vector<Chance> Chances = {
            Chance("Chances/Шанс1.png", 1000, -1), // +
            Chance("Chances/Шанс2.png", -100, -1), // -
            Chance("Chances/Шанс3.png", -1000, -1), // -
            Chance("Chances/Шанс4.png", 0, 39),  // +-
            Chance("Chances/Шанс5.png", -150, -1), // -
            Chance("Chances/Шанс6.png", -500, -1), // -
            Chance("Chances/Шанс7.png", 150, -1), // +
            Chance("Chances/Шанс8.png", 100, -1), // +
            Chance("Chances/Шанс9.png", 200, 0), // +
            Chance("Chances/Шанс10.png", -200, -1), // -
            Chance("Chances/Шанс11.png", 0, -1), // !
            Chance("Chances/Шанс12.png", 0, -1), // !
            Chance("Chances/Шанс13.png", 200, -1), // +
            Chance("Chances/Шанс14.png", -50, -1), // !
            Chance("Chances/Шанс15.png", 0, -1), // !
            Chance("Chances/Шанс16.png", 500, -1) // +
        };
        Gtk::Window* parent;

        std::vector <Street> Streets_vec = { // ПОМЕНЯТЬ КООРДЫ КВАДРАТОВ
            // A - ЦЕНА ПОКУПКИ УЛИЦЫ, B - ЦЕНА ПОВЫШЕНИЯ УРОВНЯ, С - РЕНТА НЕТ ДОМОВ, D - 1 ДОМ, E - 2 ДОМA, F - 3 ДОМА, G - 4 ДОМА, H - ОТЕЛЬ
            //    КООРДИНАТЫ                ПУТЬ                     ЦВЕТ     A   B  Коорд квадрата C   D   E   F   G    H
            Street(1635, 1735, 1125, 1307, "Streets/Строителей.jpg", "Brown", 60, 50, 1636, 1277,  {2, 10, 30, 90, 160, 250}), // down, left
            Street(1433, 1532, 1125, 1307, "Streets/Областная.jpg", "Brown", 60, 50, 1434, 1277, {4, 20, 60, 180, 320, 450}),
            Street(1130, 1229, 1125, 1307, "Streets/Товарищеский.jpg", "Light_blue", 100, 50, 1131, 1277, {6, 30, 90, 270, 400, 550}),
            Street(928, 1027, 1125, 1307, "Streets/Дыбенко.jpg", "Light_blue", 100, 50, 928, 1277, {6, 30, 90, 270, 400, 550}),
            Street(827, 926, 1125, 1307, "Streets/Солидарности.jpg", "Light_blue", 120, 50, 827, 1277, {8, 40, 100, 300, 450, 600}),
            
            Street(643, 825, 1024, 1124, "Streets/Коллонтай.jpg", "Pink", 140, 100, 644, 1025, {10, 50, 150, 450, 625, 750}),
            Street(643, 825, 923, 1023, "Streets/Российский.jpg", "Pink", 140, 100, 644, 924, {10, 50, 150, 450, 625, 750}),
            Street(643, 825, 721, 821, "Streets/Народная.jpg", "Pink", 160, 100, 644, 722, {12, 60, 180, 500, 700, 900}), 
            Street(643, 825, 519, 619, "Streets/октябрьская наб.jpg", "Orange", 180, 100, 644, 520, {14, 70, 200, 550, 750, 950}),
            Street(643, 825, 418, 518, "Streets/Славы.jpg", "Orange", 180, 100, 644, 419, {14, 70, 200, 550, 750, 950}),
            Street(643, 825, 216, 316, "Streets/Большевиков.jpg", "Orange", 200, 100, 644, 217, {16, 80, 220, 600, 800, 1000}),

            Street(826, 926, 31, 214, "Streets/Вознесенский.jpg", "Red", 220, 150, 895, 33, {18, 90, 250, 700, 875, 1050}), // up, right
            Street(1028, 1128, 31, 214, "Streets/Гороховая.jpg", "Red", 220, 150, 1098, 33, {18, 90, 250, 700, 875, 1050}),
            Street(1129, 1229, 31, 214, "Streets/Суворовский.jpg", "Red", 240, 150, 1198, 33, {20, 100, 300, 750, 925, 1100}),
            Street(1331, 1431, 31, 214, "Streets/Московский.jpg", "Yellow", 260, 150, 1401, 33, {22, 110, 330, 800, 975, 1150}),
            Street(1432, 1532, 31, 214, "Streets/Восстания.jpg", "Yellow", 260, 150, 1502, 33, {22, 110, 330, 800, 975, 1150}),
            Street(1634, 1734, 31, 214, "Streets/Лиговский.jpg", "Yellow", 280, 150, 1704, 33, {24, 120, 360, 850, 1025, 1200}),
            
            Street(1736, 1917, 216, 316, "Streets/Литейный.jpg", "Green", 300, 200, 1887, 285, {26, 130, 390, 900, 1100, 1275}),
            Street(1736, 1917, 317, 417, "Streets/Садовая.jpg", "Green", 300, 200, 1887, 386, {26, 130, 390, 900, 1100, 1275}),
            Street(1736, 1917, 519, 619, "Streets/Дворцовая.jpg", "Green", 320, 200, 1887, 588, {28, 150, 390, 900, 1200, 1400}),
            Street(1736, 1917, 822, 922, "Streets/Невский.jpg", "Blue", 350, 200, 1887, 891, {35, 175, 500, 1100, 1300, 1500}),
            Street(1736, 1917, 1024, 1124, "Streets/Дворцовая пл.jpg", "Blue", 350, 200, 1887, 1093, {50, 175, 500, 1100, 1700, 2000})

        };

        std::map <std::string, std::vector <Street*>> Color_Map = {
            {"Brown" , {&Streets_vec.at(0), &Streets_vec.at(1)}},
            {"Light_blue" , {&Streets_vec.at(2), &Streets_vec.at(3), &Streets_vec.at(4)}},
            {"Pink" , {&Streets_vec.at(5), &Streets_vec.at(6), &Streets_vec.at(7)}},
            {"Orange" , {&Streets_vec.at(8), &Streets_vec.at(9), &Streets_vec.at(10)}},
            {"Red" , {&Streets_vec.at(11), &Streets_vec.at(12), &Streets_vec.at(13)}},
            {"Yellow" , {&Streets_vec.at(14), &Streets_vec.at(15), &Streets_vec.at(16)}},
            {"Green" , {&Streets_vec.at(17), &Streets_vec.at(18), &Streets_vec.at(19)}},
            {"Blue" , {&Streets_vec.at(20), &Streets_vec.at(21)}}
        };

    public:
        // Функция для обновления информации о текущем игроке
        // Безопасная заглушка: просто перерисовываем экран. Вся логика отображения
        // текущего/следующего игрока реализована в on_draw на основе cur_player
        void update_current_player_info() {
            queue_draw();
        }

        // Функция для создания диалога ввода имен игроков
        void setup_player_names() {
            std::vector<std::string> color_names = {"Красного цвета", "Зеленого цвета", "Синего цвета", "Желтого цвета"};
            
            for (int i = 0; i < Chips.size(); i++) {
                Gtk::Dialog name_dialog("Имя игрока", *parent, true);
                name_dialog.add_button("OK", Gtk::RESPONSE_OK);
                name_dialog.add_button("Отмена", Gtk::RESPONSE_CANCEL);
                
                Gtk::Box* content = name_dialog.get_content_area();
                
                Gtk::Label label("Введите имя для игрока " + color_names[i] + ":");
                label.set_markup("<span font=\"Arial 16\" weight=\"bold\">Введите имя для игрока " + color_names[i] + ":</span>");
                
                Gtk::Entry entry;
                entry.set_size_request(300, 40);
                entry.set_text("Игрок " + std::to_string(i + 1));
                
                content->pack_start(label, false, false, 10);
                content->pack_start(entry, false, false, 10);
                
                name_dialog.set_size_request(400, 150);
                name_dialog.set_resizable(false);
                
                // Центрируем диалог
                Gdk::Rectangle rect;
                get_display()->get_primary_monitor()->get_geometry(rect);
                name_dialog.move(rect.get_x() + (rect.get_width() - 400) / 2, 
                               rect.get_y() + (rect.get_height() - 150) / 2);
                
                label.show();
                entry.show();
                name_dialog.show_all();
                
                int result = name_dialog.run();
                if (result == Gtk::RESPONSE_OK) {
                    std::string name = entry.get_text();
                    if (!name.empty()) {
                        Chips[i].name = name;
                    } else {
                        Chips[i].name = "Игрок " + std::to_string(i + 1);
                    }
                } else {
                    Chips[i].name = "Игрок " + std::to_string(i + 1);
                }
                
                // Обновляем экран сразу после ввода имени
                queue_draw();
            }
        }

        Drawing_panel(Gtk::Window* parent_){
            parent = parent_;
            // Безопасная загрузка ресурсов относительно рабочего каталога
            auto load_required = [](const std::string& path)->Glib::RefPtr<Gdk::Pixbuf>{
                if (!Glib::file_test(path, Glib::FILE_TEST_EXISTS)) {
                    throw std::runtime_error("Ресурс не найден: " + path);
                }
                return Gdk::Pixbuf::create_from_file(path);
            };
            Area = Gdk::Pixbuf::create_from_file("Streets/Поле.jpg");
            Red_Credit_Card = Gdk::Pixbuf::create_from_file("Streets/Red_Credit_Card.jpg");
            Green_Credit_Card = Gdk::Pixbuf::create_from_file("Streets/Green_Credit_Card.jpg");
            Blue_Credit_Card = Gdk::Pixbuf::create_from_file("Streets/Blue_Credit_Card.jpg");
            Yellow_Credit_Card = Gdk::Pixbuf::create_from_file("Streets/Yellow_Credit_Card.jpg");
            // Area = load_required("Streets/Поле.jpg");
            // Red_Credit_Card = load_required("Streets/Red_Credit_Card.jpg");
            // Green_Credit_Card = load_required("Streets/Green_Credit_Card.jpg");
            // Blue_Credit_Card = load_required("Streets/Blue_Credit_Card.jpg");
            // Yellow_Credit_Card = load_required("Streets/Yellow_Credit_Card.jpg");

            Red_Credit_Card = Red_Credit_Card -> scale_simple(583, 385, Gdk::INTERP_BILINEAR);
            Green_Credit_Card = Green_Credit_Card -> scale_simple(583, 385, Gdk::INTERP_BILINEAR);
            Blue_Credit_Card = Blue_Credit_Card -> scale_simple(583, 385, Gdk::INTERP_BILINEAR);
            Yellow_Credit_Card = Yellow_Credit_Card -> scale_simple(583, 385, Gdk::INTERP_BILINEAR);

            add_events(Gdk::POINTER_MOTION_MASK);
            add_events(Gdk::KEY_PRESS_MASK);
            signal_motion_notify_event().connect(sigc::mem_fun(*this, &Drawing_panel::motion_func));
            signal_key_press_event().connect(sigc::mem_fun(*this, &Drawing_panel::bones_throw), false);
            signal_key_release_event().connect(sigc::mem_fun(*this, &Drawing_panel::on_release), false);

            
            set_can_focus(true);

            set_size_request(2560, 1440);
            Area = Area -> scale_simple(1340, 1340, Gdk::INTERP_BILINEAR);
            grab_focus();
        };

        void decrement_player_index_dependencies(int removed_index){
            // Пересчитать владельцев улиц после удаления игрока
            for (auto& st : Streets_vec){
                if (st.owner == removed_index){
                    st.owner = -1;
                    st.level = 0;
                } else if (st.owner > removed_index){
                    st.owner -= 1;
                }
            }
        }

        bool on_release(GdkEventKey* event){
            if(event->keyval == GDK_KEY_space) {
                space_pressed = false;
                return true;
            }
            return false;
        }

        bool Check_neighbourhood(const Street& House, int Player){
            std::string colori = House.colour;
            std::vector <Street*> neighbourhood = Color_Map.at(colori);
            int c = 0;
            for(int i = 0; i < neighbourhood.size(); i++){
                if(neighbourhood.at(i)->owner == Player)
                    c++;
            }
            return c == neighbourhood.size();
        }

        void Valida(const Street& House){
            std::string colori = House.colour;
            std::vector <Street*> neighbourhood = Color_Map.at(colori);
            int c = 0;
            for(int i = 0; i < neighbourhood.size(); i++){
                if(!neighbourhood.at(i)->monopoly_bonus_applied){
                    neighbourhood.at(i)->rent.at(0) *= 2;
                    neighbourhood.at(i)->monopoly_bonus_applied = true;
                }
            }

        }

        void apply_chance(const Chance& Ch, Chip& player, int PL){
            
            if(Ch.Chance_path.find("11.png") != -1){
                player.A_Way_Out = 1;
            }
            else if(Ch.Chance_path.find("12.png") != -1){
                for(int i = 0; i < Streets_vec.size(); i++){
                    Street St = Streets_vec.at(i);
                    if(St.owner == PL){
                        if(St.level <= 4)
                            player.money -= St.level * 250;
                        else
                            player.money -= 1000;
                    }
                }
            }
            else if(Ch.Chance_path.find("14.png") != -1){
                for(int i = 0; i < Chips.size(); i++){
                    if(&Chips.at(i) != &player){
                        Chips.at(i).money -= Ch.money_for_pay;
                        player.money += Ch.money_for_pay;
                    }
                }
            }
            else if(Ch.Chance_path.find("15.png") != -1){
                Jail(player);
            }
            else{
                player.money += Ch.money_for_pay;
                if(Ch.pos_to_move != - 1){
                    move_on(player, Ch.pos_to_move);
                    // После перемещения по карте шанса — обработать клетку
                    int idx = &player - &Chips[0];
                    show_win_for_action(idx);
                }
                    
                
            }
            queue_draw();
        }
        void give_money(int cur_player_){
            Chip &now_pl = Chips.at(cur_player_);

            if(now_pl.pos == 4 or now_pl.pos == 22)
                now_pl.money += 50;
            else
                now_pl.money -= 100;
            
        }

        void ChA(int cur_player_){
            Chip &now_pl = Chips.at(cur_player_);
            std::mt19937 gen(rd());
            std::uniform_int_distribution<int> distrib(0, 15);
            int io = distrib(gen);
            if (!Glib::file_test(Chances.at(io).Chance_path, Glib::FILE_TEST_EXISTS)) {
                std::cerr << "Ошибка: файл " << Chances.at(io).Chance_path << " не найден!" << std::endl;
                return;
            }
            
            Gtk::Dialog dialog_ch("Шанс", *parent, true);
            dialog_ch.add_button("Ok", Gtk::RESPONSE_OK);
            dialog_ch.add_button("Cancel", Gtk::RESPONSE_CANCEL);
            Gtk::Box* content = dialog_ch.get_content_area();
            Gtk::Image go (Chances.at(io).Chance_path);
            std::cout << Chances.at(io).Chance_path << std::endl;
            dialog_ch.set_size_request(854, 480);
            dialog_ch.set_resizable(false);
            Gdk::Rectangle Rect;
            get_display() -> get_primary_monitor() -> get_geometry(Rect);
            dialog_ch.move(Rect.get_x() + 860, Rect.get_y() + 441);
            content -> pack_start(go, false, false);
            go.show();
            dialog_ch.show_all();
            int res = dialog_ch.run();
            apply_chance(Chances.at(io), now_pl, cur_player_);
            grab_focus();
        }

        void show_win_for_action(int cur_player_){ 
            Chip &now_pl = Chips.at(cur_player_);

            for(Street& cur_str : Streets_vec){
                
                if(now_pl.x < cur_str.x_end and now_pl.x > cur_str.x_beg and now_pl.y < cur_str.y_end and now_pl.y > cur_str.y_beg and cur_str.owner == -1){
                    
                    Gtk::MessageDialog msgd(*parent, "Подтверждение покупки", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL, false);
                    msgd.set_secondary_text("Вы хотите купить " + cur_str.name + "? Это будет стоить " + std::to_string(cur_str.cost));
                    int res = msgd.run();
                    // std::cout << res << std::endl;
                    if(res == Gtk::RESPONSE_OK && now_pl.money >= cur_str.cost){
                        cur_str.owner = cur_player_;
                        Chips.at(cur_player_).money -= cur_str.cost;
                        // std::cout << cur_str.owner << std::endl;
                    }
                    if(Check_neighbourhood(cur_str, cur_player_))
                        Valida(cur_str);
                    break;
                }
                else if (now_pl.x < cur_str.x_end and now_pl.x > cur_str.x_beg and now_pl.y < cur_str.y_end and now_pl.y > cur_str.y_beg and cur_str.owner == cur_player_ and cur_str.level != 5)
                {
                    Gtk::MessageDialog msgd(*parent, "Повышение уровня", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL, false);
                    msgd.set_secondary_text("Вы хотите улучшить улицу до " + std::to_string(cur_str.level + 1) + " уровня? Это будет стоить " + std::to_string(cur_str.cost_for_lvl_up));
                    int res = msgd.run();
                    if(res == Gtk::RESPONSE_OK && now_pl.money >= cur_str.cost_for_lvl_up){
                        Chips.at(cur_player_).money -= cur_str.cost_for_lvl_up;
                        cur_str.level++;
                    }
                }
                else if (now_pl.x < cur_str.x_end and now_pl.x > cur_str.x_beg and now_pl.y < cur_str.y_end and now_pl.y > cur_str.y_beg and cur_str.owner != -1 and cur_str.owner != cur_player_ and Chips.at(cur_str.owner).Jail == 0)
                {
                    Gtk::MessageDialog msgd(*parent, "Оплата ренты", false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_OK_CANCEL, false);
                    msgd.set_secondary_text("Вы должны заплатить владельцу " + std::to_string(cur_str.rent.at(cur_str.level)));
                    int res = msgd.run();
                    if(res == Gtk::RESPONSE_OK && now_pl.money >= cur_str.rent.at(cur_str.level)){
                        // std::cout << Chips.at(cur_str.owner).money << "  " << now_pl.money << std::endl;
                        Chips.at(cur_str.owner).money += cur_str.rent.at(cur_str.level);
                        Chips.at(cur_player_).money -= cur_str.rent.at(cur_str.level);
                        // std::cout << Chips.at(cur_str.owner).money << "  " << now_pl.money << std::endl;
                    }
                    else
                    {
                        Chips.erase(Chips.begin() + cur_player_);
                        decrement_player_index_dependencies(cur_player_);
                        if (cur_player >= static_cast<int>(Chips.size())) {
                            cur_player = 0;
                        }
                        
                    }
                    
                }
                
                
            }
            grab_focus();
        }
        void Jail(Chip& Pl){
            // Перемещаем игрока в тюрьму (клетка 10), используя стандартную механику клеток
            animation_in_progress = true;  // Начинаем анимацию перемещения в тюрьму
            
            if (Cells.at(Pl.pos).count_of_pl > 0) {
                Cells.at(Pl.pos).count_of_pl--;
            }
            Pl.pos = 10;
            auto& cur_cell_slots = Cells.at(Pl.pos).slots;
            Cell cur_slot = cur_cell_slots.at(Cells.at(Pl.pos).count_of_pl);
            Pl.x = cur_slot.x;
            Pl.y = cur_slot.y;
            Cells.at(Pl.pos).count_of_pl++;
            Pl.Jail = 1;
            queue_draw();
            
            // Небольшая задержка для визуализации перемещения в тюрьму
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            animation_in_progress = false;  // Заканчиваем анимацию
        }
        void move_on(Chip& Pl, int dest_pos){
            animation_in_progress = true;  // Начинаем анимацию перемещения
            
            // Если перемещение назад через старт — начислить 200
            if (dest_pos < Pl.pos) {
                Pl.money += 200;
            }
            auto& cur_Cell = Cells.at(dest_pos).slots; //Список слотов текущей клетки

            if (Cells.at(Pl.pos).count_of_pl > 0) {
                Cells.at(Pl.pos).count_of_pl--;// убираем из предыдущей ячейки
            }
            Pl.pos = dest_pos;
            Cell cur_slot = cur_Cell.at(Cells.at(dest_pos).count_of_pl);
            Pl.x = cur_slot.x;
            Pl.y = cur_slot.y;
            Cells.at(dest_pos).count_of_pl++;// добавляем в новой ячейке

            queue_draw();
            
            // Небольшая задержка для визуализации перемещения
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            animation_in_progress = false;  // Заканчиваем анимацию

        }

bool bones_throw(GdkEventKey* space) {
    try {
        // Проверяем, была ли нажата клавиша пробела
        if (space->keyval == GDK_KEY_space) {
            // Проверяем, что это событие нажатия (не отпускания) и что пробел не был уже нажат
            // Также проверяем, что анимация не в процессе
            if (space->type == GDK_KEY_PRESS and !space_pressed and !animation_in_progress) {
                std::vector <int> poss = {2, 4, 22, 33};
                std::vector <int> posss = {7, 18, 28, 36};
                // Если игрок в тюрьме
                if(Chips.at(cur_player).A_Way_Out > 0 and Chips.at(cur_player).Jail != 0){
                    animation_in_progress = true;  // Блокируем ввод во время диалога
                    
                    Gtk::MessageDialog msgd(*parent, "Выход из тюрьмы", false, Gtk::MESSAGE_INFO, Gtk::BUTTONS_OK, false);
                    msgd.set_secondary_text("У вас была карта освобождения из тюрьмы. Вы свободны!");
                    msgd.run();
                    Chips.at(cur_player).Jail = 0;
                    Chips.at(cur_player).A_Way_Out -= 1;
                    animation_in_progress = false;  // Разблокируем ввод
                    
                    grab_focus();
                }
                if (Chips.at(cur_player).Jail != 0) {
                    animation_in_progress = true;  // Блокируем ввод во время диалога
                    
                    // Создаем диалоговое окно для выхода из тюрьмы
                    Gtk::Dialog A_Way_Out("Выход", *parent, true);
                    A_Way_Out.add_button("Больше 8 очков", Gtk::RESPONSE_OK);
                    A_Way_Out.add_button("50 Руб", Gtk::RESPONSE_CANCEL);
                    int res = A_Way_Out.run();
                    A_Way_Out.close();
                    
                    // Если выбран вариант "Больше 8 очков"
                    if (res == Gtk::RESPONSE_OK) {
                        // Даем 3 попытки бросить кости
                        for (int i = 0; i < 3; i++) {
                            if (space->keyval == GDK_KEY_space) {
                                if (space->type == GDK_KEY_PRESS and !space_pressed) {
                                    // Генерируем случайное число от 2 до 12
                                    std::mt19937 gen(rd());
                                    std::uniform_int_distribution<int> distribu(2, 12);
                                    bounce = distribu(gen);
                                    queue_draw();  // Обновляем отрисовку
                                    while (gtk_events_pending()) gtk_main_iteration();
                                    
                                    // Если выпало больше 8 - освобождаем из тюрьмы
                                    if (bounce > 8) {
                                        Chips.at(cur_player).Jail = 0;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    else {  // Если выбран вариант "50 Руб"
                        if (Chips.at(cur_player).money >= 50) {
                            Chips.at(cur_player).money -= 50;
                            Chips.at(cur_player).Jail = 0;  // Освобождаем за 50 рублей
                        }
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500));  // Задержка
                    animation_in_progress = false;  // Разблокируем ввод
                    grab_focus();
                }
                
                // Если игрок не в тюрьме
                if (Chips.at(cur_player).Jail == 0) {
                    space_pressed = true;
                    
                    animation_in_progress = true;  // Начинаем анимацию
                    // Генерируем количество ходов
                    std::mt19937 gen(rd());
                    std::uniform_int_distribution<int> distrib(2, 12);
                    bounce = distrib(gen);
                    
                    // Фиксируем игрока для отображения хода и перерисовываем
                    displayed_turn_player_index = cur_player;
                    // Рассчитываем целевую клетку для подсветки
                    {
                        int target_pos = Chips.at(cur_player).pos + bounce;
                        // учёт прохождения круга
                        target_pos %= 40;
                        // нормализация позиций кратных 10 (углы как у вас в анимации)
                        if (target_pos % 10 == 0) {
                            target_pos = (target_pos / 10) * 10;
                        }
                        highlighted_cell_index = target_pos;
                    }
                    queue_draw();
                    
                    // Сохраняем текущее состояние
                    Chip c_p = Chips.at(cur_player);
                    std::vector<Cell> cls = Cells;
                    std::cout << "Текущая позиция: " << Chips.at(cur_player).pos << " Бросков: " << bounce << std::endl;
                    
                    // Выполняем перемещение на каждое значение броска
                    for (int t = 0; t < bounce; t++) {
                        // Убираем фишку с текущей позиции
                        Cells.at(Chips.at(cur_player).pos).count_of_pl--;
                        
                        // Перемещаем фишку вперед
                        Chips.at(cur_player).pos += 1;
                        
                        // Если прошли круг - получаем 200 рублей
                        if (Chips.at(cur_player).pos >= 40) {
                            Chips.at(cur_player).money += 200;
                        }
                        
                        // Зацикливаем позицию (если >40)
                        Chips.at(cur_player).pos %= 40;
                        
                        // Специальная обработка для позиций, кратных 10
                        if (Chips.at(cur_player).pos % 10 == 0) {  
                            Chips.at(cur_player).pos /= 10;
                            Chips.at(cur_player).pos *= 10;
                        }

                        // Обновляем состояние
                        c_p = Chips.at(cur_player);
                        cls = Cells;
                        
                        // Получаем слот для новой позиции
                        std::vector<Cell> cur_Cell = Cells.at(Chips.at(cur_player).pos).slots;
                        Cell cur_slot = cur_Cell.at(Cells.at(Chips.at(cur_player).pos).count_of_pl);
                        
                        // Устанавливаем новые координаты фишки
                        Chips.at(cur_player).x = cur_slot.x;
                        Chips.at(cur_player).y = cur_slot.y;
                        
                        // Добавляем фишку на новую позицию
                        Cells.at(Chips.at(cur_player).pos).count_of_pl++;
                        
                        // Обновляем отрисовку и делаем небольшую задержку
                        queue_draw();
                        while (gtk_events_pending()) gtk_main_iteration();
                        std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    }
                    
                    // Если попали на 30 позицию - отправляем в тюрьму
                    if (Chips.at(cur_player).pos == 30) {
                        Jail(Chips.at(cur_player));
                    }

                    if(std::find(poss.begin(), poss.end(), Chips.at(cur_player).pos) != poss.end())
                        give_money(cur_player);

                    if(std::find(posss.begin(), posss.end(), Chips.at(cur_player).pos) != posss.end())
                        ChA(cur_player);
                    
                    // Передаем ход следующему игроку
                    std::cout << "Новая позиция: " << Chips.at(cur_player).pos << std::endl;
                    int cur_player_before = cur_player;
                    cur_player++;
                    if (!Chips.empty()) cur_player %= Chips.size();
                    space_pressed = true;
                    // Показываем результат хода
                    show_win_for_action(cur_player_before);
                    animation_in_progress = false;  // Заканчиваем анимацию
                    // Убираем подсветку после завершения хода
                    highlighted_cell_index = -1;
                    queue_draw();
                    grab_focus();
                }
                else if (space_pressed == true) {
                    space_pressed = false;
                }
            }
        }
    } catch (std::out_of_range& ex) {
        std::cout << "Ошибка доступа к элементу: " << ex.what() << std::endl;
    }
    
    // Обновляем отрисовку и возвращаем true
    queue_draw();
    return true;
}


        // Отслеживание
        bool motion_func(GdkEventMotion* MotEvt){
            int temp_x = MotEvt -> x;
            int temp_y = MotEvt -> y;
            // std::cout << temp_x << "    " << temp_y << std::endl;
            for(int i = 0; i < Streets_vec.size(); i++){
                if(Streets_vec.at(i).Check_str(temp_x, temp_y)){
                    Card_img = load_pixbuf_scaled_cached(Streets_vec.at(i).Path, 380, static_cast<int>(380 * 1.4141935));
                    queue_draw();
                    
                    break;
                }
                if(i == Streets_vec.size() - 1){
                    Card_img.reset();
                    queue_draw();
                }
            }
            return 1;
        }

        bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override{
            
            Gdk::Cairo::set_source_pixbuf(cr, Area, 610, 0);
            cr -> paint();
            
            Gdk::Cairo::set_source_pixbuf(cr, Red_Credit_Card, 0, 0);
            cr -> paint();
            Gdk::Cairo::set_source_pixbuf(cr, Green_Credit_Card, 1977, 955);
            cr -> paint();
            if (Chips.size() == 3 or Chips.size() == 4)
            {
                Gdk::Cairo::set_source_pixbuf(cr, Blue_Credit_Card, 1977, 0);
                cr -> paint();
            }
            if (Chips.size() == 4)
            {
                Gdk::Cairo::set_source_pixbuf(cr, Yellow_Credit_Card, 0, 955);
                cr -> paint();
            }

            cr -> move_to(110, 724);
            cr -> select_font_face("monospace", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
            cr -> set_font_size(50);
            cr->set_source_rgb(0, 0, 0);
            if(bounce == 0)
                cr -> show_text("Приятной игры)");
            else
                cr -> show_text("Выпало:  " + std::to_string(bounce));
            cr->fill();
            cr -> move_to(110, 589);
            cr -> set_font_size(40);

            // Показываем информацию о ходе только когда мы её зафиксировали перед анимацией
            if (displayed_turn_player_index >= 0 && !Chips.empty()) {
                int cur_idx = displayed_turn_player_index % Chips.size();
                if (cur_idx < 0) cur_idx += Chips.size();
                int next_idx = (cur_idx + 1) % Chips.size();

                std::string current_player_name = Chips.at(cur_idx).name.empty() ? "Игрок " + std::to_string(cur_idx + 1) : Chips.at(cur_idx).name;
                std::string next_player_name = Chips.at(next_idx).name.empty() ? "Игрок " + std::to_string(next_idx + 1) : Chips.at(next_idx).name;

                cr -> show_text("Текущий ход: " + current_player_name);
                cr -> move_to(110, 649);
                cr -> show_text("Следующий ход: " + next_player_name);
            } else {
                cr -> show_text("");
            }
            
            cr->fill();

            // Подсветка целевой клетки
            if (highlighted_cell_index >= 0 && highlighted_cell_index < (int)Cells.size()) {
                const auto &cell = Cells.at(highlighted_cell_index);
                // Центр клетки (по первому слоту или по середине слотов)
                int hx = cell.slots.empty() ? cell.x : cell.slots.front().x;
                int hy = cell.slots.empty() ? cell.y : cell.slots.front().y;
                cr->set_source_rgba(1.0, 0.84, 0.0, 0.35); // полупрозрачный жёлтый
                cr->arc(hx, hy, 38, 0, 2*3.14159265358979323846);
                cr->fill();
            }
            cr->set_source_rgb(1, 1, 1);
            cr -> select_font_face("monospace", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD); // Деньги
            cr -> set_font_size(70);
            for(int i = 0; i < Chips.size(); i++){
                if(i == 0)
                    cr -> move_to(92, 300);
                if(i == 1)
                    cr -> move_to(2070, 1256);
                if(i == 2)
                    cr -> move_to(2070, 300);
                if(i == 3)
                    cr -> move_to(92, 1256);
                cr -> show_text(std::to_string(Chips.at(i).money));
                cr->fill();
            }
            
            // Отображение имен игроков на банковских карточках
            cr->set_source_rgb(0, 0, 0);
            cr -> select_font_face("Courier New", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_BOLD);
            cr -> set_font_size(35);
            for(int i = 0; i < Chips.size(); i++){
                std::string player_name = Chips.at(i).name.empty() ? "Игрок " + std::to_string(i + 1) : Chips.at(i).name;
                
                if(i == 0) {
                    cr -> move_to(92, 95);  // Позиция над магнитной полоской
                    cr -> show_text(player_name);
                }
                if(i == 1) {
                    cr -> move_to(2070, 1051);  // Позиция над магнитной полоской
                    cr -> show_text(player_name);
                }
                if(i == 2) {
                    cr -> move_to(2070, 95);  // Позиция над магнитной полоской
                    cr -> show_text(player_name);
                }
                if(i == 3) {
                    cr -> move_to(92, 1051);  // Позиция над магнитной полоской
                    cr -> show_text(player_name);
                }
                cr->fill();
            }
            
            for(Street& cur_str : Streets_vec){ // квадраты хазяева
                if(cur_str.owner != -1){
                    Chip Chip_owner = Chips.at(cur_str.owner);

                    double R_sq = Chip_owner.R;
                    double G_sq = Chip_owner.G;
                    double B_sq = Chip_owner.B;
                    cr -> set_source_rgb(R_sq / 255, G_sq / 255, B_sq / 255);
                    cr -> rectangle(cur_str.X_for_sq, cur_str.Y_for_sq, 30, 30);
                    cr->fill();
                    auto pair_lvl = cur_str.coord_for_lvl();
                    cr -> move_to(pair_lvl.first, pair_lvl.second);
                    cr -> select_font_face("monospace", Cairo::FONT_SLANT_NORMAL, Cairo::FONT_WEIGHT_NORMAL);
                    cr -> set_font_size(26);
                    cr->set_source_rgb(1, 1, 1);
                    cr -> show_text(std::to_string(cur_str.level));
                    cr->fill();
                }
            }

            if(!Chips.empty()){
                for (int i = 0; i < Chips.size(); i++){
                    cr -> set_source_rgba(0, 0, 0, 0.5);
                    cr -> arc(Chips.at(i).x + 3, Chips.at(i).y + 3, 20, 0, 2*3.14159265358979323846);
                    cr -> fill();
                    cr -> set_source_rgb(Chips.at(i).R / 255, Chips.at(i).G / 255, Chips.at(i).B / 255);
                    cr -> arc(Chips.at(i).x, Chips.at(i).y, 20, 0, 2*3.14159265358979323846);
                    cr -> fill();
                }
            }
            
            if(Card_img){
                Gdk::Cairo::set_source_pixbuf(cr, Card_img, 2064, 401);
                cr -> paint();
            }

            
            return true;
        }
    };


class mainWin : public Gtk::Window
{
private:
    Gtk::Button start;
    Gtk::Button exit;
    Gtk::ComboBoxText cop;
    Gtk::Label spbpoly;
    Gtk::Fixed fix_pan;
    Drawing_panel Drow_pls = Drawing_panel(this);
    Gtk::Label Bones_vision;


    
public:
    
    void set_UI(){
        start.add_label("НАЧАТЬ");
        if(auto label = dynamic_cast <Gtk::Label*> (start.get_child())){
            label -> set_markup("<span font = \"Arial 36\" color = \"black\" >НАЧАТЬ</span>");
        }
        
        start.set_size_request(260, 120);
        exit.add_label("ВЫЙТИ");
        exit.set_size_request(100, 25);

        cop.append("2 Человека");
        cop.append("3 Человека");
        cop.append("4 Человека");
        cop.set_size_request(260, 120);
        cop.set_active(2);
        if(auto label = dynamic_cast <Gtk::CellRendererText*> (cop.get_first_cell())){
            label -> property_font() = "Arial 30";
        }
        Bones_vision.set_text("Приятной игры");
        Bones_vision.set_markup("<span font = \"Arial 24\" weight = \"Bold\" color = \"black\" >Приятной игры</span>");
// Bones_vision.set_markup("<span font = \"Arial 24\" weight = \"Bold\" color = \"black\"" + to_string(rand) + "</span>");
        spbpoly.set_markup("<span font=\"Arial 72\" weight=\"bold\" color=\"black\">SPbPOLY</span>");
        fix_pan.put(spbpoly, 420, 150);
        fix_pan.put(cop, 510, 300);

        fix_pan.put(start, 510, 460);

        fix_pan.put(Drow_pls, 0, 50);
        fix_pan.put(exit, 1230, 13);
        // fix_pan.put(Bones_vision, 140, 550);

        Drow_pls.set_visible(0);
        exit.set_visible(0);
        Bones_vision.set_visible(0);
        this -> add(fix_pan);
    }


    // Реагирование на сигнал
    void listeners(){
        start.signal_clicked().connect(sigc::mem_fun(*this, &mainWin::check));
        exit.signal_clicked().connect(sigc::mem_fun(*this, &mainWin::exit_func));
    }

    
    // выйти
    void exit_func(){
        hide();
    }


    // После нажатие на Start
    void check(){
        spbpoly.hide();
        std::string cop_decision = cop.get_active_text();
        cop.hide();
        start.hide();
        set_resizable(true);
        fullscreen();

        Bones_vision.set_visible(1);
        exit.set_visible(1);

        
        if(cop_decision == "2 Человека"){
            Chips = {Chip(1807, 1196, 204, 6, 5,  'R'), Chip(1877, 1266, 154, 205, 50,  'G')};
            Cells.at(0).count_of_pl = 2;
        }
        else if(cop_decision == "3 Человека"){
            Chips = {Chip(1807, 1196, 204, 6, 5,  'R'), Chip(1877, 1266, 154, 205, 50,  'G'), Chip(1807, 1266, 0, 103, 165,  'B')};
            Cells.at(0).count_of_pl = 3;
        }
        else if(cop_decision == "4 Человека"){
            Chips = {Chip(1807, 1196, 204, 6, 5,  'R'), Chip(1877, 1266, 154, 205, 50,  'G'), Chip(1807, 1266, 0, 103, 165,  'B'), Chip(1877, 1196, 244, 169, 0,  'Y')};
            Cells.at(0).count_of_pl = 4;
        }

        Drow_pls.set_visible(1);
        
        // Запрашиваем имена игроков
        Drow_pls.setup_player_names();
        
        // Обновляем информацию о текущем игроке после ввода имен
        Drow_pls.update_current_player_info();
        
        Drow_pls.grab_focus();
        Drow_pls.queue_draw();
    }

    // Окно
    mainWin(){
        set_default_size(1280, 720);
        set_title("SPbPoly");
        set_resizable(false);
        
        set_UI();

        listeners();

        show_all_children(0);
        cop.set_visible(1);
        start.set_visible(1);
        spbpoly.set_visible(1);

        grab_focus();
    }
   
};


int main(int argc, char **argv){
// Gtk::  Виджеты
// Gdk::  Типы данных
// Glib:  Более сложное 

    // Пути к картинкам заданы относительно папки с игрой (Streets/, Chances/).
    // Переходим в папку исполняемого файла, чтобы игра работала независимо от того,
    // из какого каталога её запустили (двойной клик, VS Code, терминал).
    try {
        if(argc > 0 && argv[0] != nullptr){
            std::filesystem::path exe_dir = std::filesystem::canonical(argv[0]).parent_path();
            if(std::filesystem::exists(exe_dir / "Streets"))
                std::filesystem::current_path(exe_dir);
        }
    } catch(const std::exception&){
        // папку определить не удалось — работаем с текущей
    }

    auto app = Gtk::Application::create(argc, argv, "SPbPoly");
  
    mainWin window;
    return app -> run(window);
}