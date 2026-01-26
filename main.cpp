// main.cpp
#include <sl/Camera.hpp>
#include <iostream>
#include <cmath> // Để tính căn bậc 2 (khoảng cách)

using namespace sl;
using namespace std;

int main(int argc, char **argv) {
    // 1. KHỞI TẠO CAMERA
    Camera zed;
    InitParameters init_parameters;
    init_parameters.camera_resolution = RESOLUTION::HD720; // 720p là đủ cho TX2
    init_parameters.camera_fps = 30;
    init_parameters.depth_mode = DEPTH_MODE::PERFORMANCE; // Quan trọng: Chế độ nhẹ nhất
    init_parameters.coordinate_units = UNIT::METER;       // Đơn vị mét
    init_parameters.coordinate_system = COORDINATE_SYSTEM::RIGHT_HANDED_Y_UP;

    // Mở camera
    ERROR_CODE err = zed.open(init_parameters);
    if (err != ERROR_CODE::SUCCESS) {
        cout << "Loi: Khong mo duoc Camera (" << err << "). Check day cap USB!" << endl;
        return 1;
    }

    // 2. BẬT TÍNH NĂNG PHÁT HIỆN NGƯỜI (Object Detection)
    ObjectDetectionParameters obj_param;
    obj_param.enable_tracking = true; // Bật theo dõi ID
    obj_param.enable_segmentation = false; // Tắt segmentation cho nhẹ RAM
    
    // Dùng model MULTI_CLASS_BOX_FAST để tối ưu tốc độ cho TX2
    obj_param.detection_model = DETECTION_MODEL::MULTI_CLASS_BOX_FAST;

    ERROR_CODE err_od = zed.enableObjectDetection(obj_param);
    if (err_od != ERROR_CODE::SUCCESS) {
        cout << "Loi: Khong bat duoc Object Detection (" << err_od << ")" << endl;
        zed.close();
        return 1;
    }

    // 3. CẤU HÌNH BỘ LỌC (CHỈ LẤY NGƯỜI)
    ObjectDetectionRuntimeParameters obj_runtime_param;
    obj_runtime_param.detection_confidence_threshold = 40;
    obj_runtime_param.object_class_filter.push_back(OBJECT_CLASS::PERSON);

    Objects objects; // Biến chứa danh sách vật thể

    cout << ">>> ROBOT VISION C++ STARTED (Nhan Ctrl+C de thoat) <<<" << endl;

    // 4. VÒNG LẶP CHÍNH
    while (true) {
        if (zed.grab() == ERROR_CODE::SUCCESS) {
            // Lấy dữ liệu vật thể
            zed.retrieveObjects(objects, obj_runtime_param);

            if (objects.is_new) {
                // Duyệt qua danh sách người tìm thấy
                for (auto& obj : objects.object_list) {
                    // Lấy ID và tọa độ
                    int id = obj.id;
                    float x = obj.position.x;
                    float y = obj.position.y;
                    float z = obj.position.z;

                    // Tính khoảng cách (Euclidean distance)
                    float distance = sqrt(x*x + y*y + z*z);

                    // In thông tin ra màn hình
                    cout << "Phat hien Nguoi ID " << id << " | Cach: " << distance << "m" << endl;

                    // --- LOGIC ROBOT TIẾP TÂN ---
                    if (distance < 1.5) {
                        cout << "   -> HANH DONG: Xin chao quy khach!" << endl;
                        // Ở đây bạn có thể gọi hàm gửi tín hiệu xuống mạch điều khiển
                    } else if (distance < 3.0) {
                        cout << "   -> HANH DONG: Chuan bi..." << endl;
                    }
                }
            }
        }
    }

    // Dọn dẹp
    zed.disableObjectDetection();
    zed.close();
    return 0;
}