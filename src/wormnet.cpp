#include <unordered_map>
#include <mutex>
#include <thread>
#include <chrono>
#include "crow.h"

class State {
    public:
    int val = 0;
    std::thread* t;
    bool runme = true;

    ~State() {
        std::thread::id t_id = t->get_id();
        runme = false;
        
        t->join();
        
        delete t;
        CROW_LOG_INFO << "[" << t_id << "] State deleted";
    }

    void run() {

        t = new std::thread(&State::autoMessage, this);
        
    }

    // Thread function
    void autoMessage() {
        while(runme) {
            val++;
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            CROW_LOG_INFO << "[" << t->get_id() << "] " << val;
        }
        CROW_LOG_INFO << "[" << t->get_id() << "] Thread stopped";
    }
};


int main()
{
    crow::SimpleApp app;

    std::mutex mtx;
    std::unordered_map<crow::websocket::connection*, State*> users;

    CROW_WEBSOCKET_ROUTE(app, "/ws")
      .onopen([&](crow::websocket::connection& conn) {
          CROW_LOG_INFO << "new websocket connection from " << conn.get_remote_ip();
          std::lock_guard<std::mutex> _(mtx);

            State* state = new State();
            state->run();

            users.insert(std::make_pair<crow::websocket::connection*, State*&>(&conn, state));
      })
      .onclose([&](crow::websocket::connection& conn, const std::string& reason) {
          CROW_LOG_INFO << "websocket connection closed: " << reason;
          std::lock_guard<std::mutex> _(mtx);
          delete users[&conn];
          users.erase(&conn);
      })
      .onmessage([&](crow::websocket::connection& conn, const std::string& data, bool is_binary) {
          std::lock_guard<std::mutex> _(mtx);
          
          crow::json::rvalue v = crow::json::load(data);

          if (v["command"] == "UPDATE_THREAD") {
            crow::json::wvalue resp({{"status", "OK"},
                                      {"value", users[&conn]->val}
            });
 
            conn.send_text(resp.dump());
          } else {
            for (auto u : users)
                if (is_binary)
                    u.first->send_binary(data);
                else
                    u.first->send_text(data);
          }
      });

    CROW_ROUTE(app, "/")
    ([] {
        char name[256];
        gethostname(name, 256);
        crow::mustache::context x;
        x["servername"] = name;

        crow::mustache::set_base("src/templates");
        auto page = crow::mustache::load("ws.html");
        return page.render(x);
    });

    app.port(40080)
      .multithreaded()
      .run();
}
