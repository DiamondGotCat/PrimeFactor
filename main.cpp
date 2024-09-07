#include <gtkmm.h>
#include <iostream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <stdint.h>

// Fortranサブルーチンの宣言
extern "C" {
    void factorize_(int64_t *n, int64_t *factors, int *num_factors);
}

// 数値をフォーマットするためのヘルパー関数
std::string int_to_string(int64_t number) {
    std::ostringstream oss;
    oss << number;
    return oss.str();
}

// メインアプリケーションクラス
class FactorizerApp : public Gtk::Window {
public:
    FactorizerApp();
    virtual ~FactorizerApp();

protected:
    // Signal handlers:
    void on_button_clicked();

    // Member widgets:
    Gtk::Box m_box;
    Gtk::Entry m_entry;
    Gtk::Button m_button;
    Gtk::Label m_result_label;
};

FactorizerApp::FactorizerApp()
: m_box(Gtk::ORIENTATION_VERTICAL),
  m_button("Run") {
    // Set up window properties
    set_title("PrimeFactor");
    set_border_width(10);
    set_default_size(300, 100);

    // Add widgets to the box (vertical layout)
    add(m_box);

    // Add entry field for number input
    m_box.pack_start(m_entry);
    m_entry.set_placeholder_text("Enter a number...");

    // Add button and connect signal handler
    m_box.pack_start(m_button);
    m_button.signal_clicked().connect(sigc::mem_fun(*this, &FactorizerApp::on_button_clicked));

    // Add label for displaying result
    m_box.pack_start(m_result_label);

    // Show all widgets
    show_all_children();
}

FactorizerApp::~FactorizerApp() {}

void FactorizerApp::on_button_clicked() {
    try {
        // Get the number from the entry field
        std::string input_text = m_entry.get_text();
        int64_t number = std::stoll(input_text); // 文字列を64ビット整数に変換

        // Prepare to receive factors and their count
        int64_t factors[100];
        int num_factors = 0;

        // Call the Fortran subroutine to factorize the number
        factorize_(&number, factors, &num_factors);

        // Format the result for display
        std::string result_text;
        if (num_factors > 0) {
            result_text = "Result: " + int_to_string(factors[0]);
            for (int i = 1; i < num_factors; ++i) {
                result_text += " * " + int_to_string(factors[i]);
            }
        } else {
            result_text = "Input must be greater than 1.";
        }

        // Display the result in the label
        m_result_label.set_text(result_text);
    } catch (const std::invalid_argument& e) {
        // 数値に変換できない入力があった場合のエラーメッセージ
        m_result_label.set_text("Error: Invalid input. Please enter a valid number.");
    } catch (const std::exception& e) {
        // 数値が範囲外の場合に表示するエラーメッセージ
        m_result_label.set_text("Error: Cannot calculate on this machine.");
    }
}

int main(int argc, char *argv[]) {
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.factorizer");

    FactorizerApp window;

    // Run the GTK application
    return app->run(window);
}
