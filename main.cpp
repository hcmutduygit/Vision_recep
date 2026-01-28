// main.cpp - PHIÊN BẢN CHO ZED SDK CŨ (v3.x)
#include <sl/Camera.hpp>
#include <iostream>
#include <cmath>

using namespace sl;
using namespace std;

int main(int argc, char **argv) {
    // 1. KHỞI TẠO CAMERA
    Camera zed;
    InitParameters init_parameters;
    init_parameters.camera_resolution = RESOLUTION::VGA;
    init_parameters.camera_fps = 15;
    init_parameters.depth_mode = DEPTH_MODE::PERFORMANCE;
    init_parameters.coordinate_units = UNIT::METER;
    init_parameters.coordinate_system = COORDINATE_SYSTEM::RIGHT_HANDED_Y_UP;

    // Mở camera
    ERROR_CODE err = zed.open(init_parameters);
    if (err != ERROR_CODE::SUCCESS) {
        cout << "Loi: Khong mo duoc Camera (" << err << ")" << endl;
        return 1;
    }

    // --- [MỚI THÊM] BƯỚC 1.5: BẬT POSITIONAL TRACKING ---
    // Bắt buộc phải có cái này thì mới Tracking người được
    PositionalTrackingParameters tracking_parameters;
    // Để mặc định là ổn
    err = zed.enablePositionalTracking(tracking_parameters);
    if (err != ERROR_CODE::SUCCESS) {
        cout << "Loi: Khong bat duoc Dinh vi (" << err << ")" << endl;
        zed.close();
        return 1;
    }
    // ----------------------------------------------------

    // 2. CẤU HÌNH OBJECT DETECTION (Sửa cho SDK cũ)
    ObjectDetectionParameters obj_param;
    obj_param.enable_tracking = true; // Vẫn giữ tracking
    
    // --- SỬA LỖI TẠI ĐÂY ---
    // Bản cũ không có 'enable_segmentation', mặc định nó đã tắt nếu dùng model BOX.
    // Xóa dòng obj_param.enable_segmentation = false; đi.

    // Thay 'MULTI_CLASS_BOX_FAST' bằng 'MULTI_CLASS_BOX' (Model chuẩn có ở mọi phiên bản)
    obj_param.detection_model = DETECTION_MODEL::MULTI_CLASS_BOX; 
    // ------------------------

    ERROR_CODE err_od = zed.enableObjectDetection(obj_param);
    if (err_od != ERROR_CODE::SUCCESS) {
        cout << "Loi: Khong bat duoc Object Detection (" << err_od << ")" << endl;
        zed.close();
        return 1;
    }


    // 3. CẤU HÌNH RUNTIME
    ObjectDetectionRuntimeParameters obj_runtime_param;
    obj_runtime_param.detection_confidence_threshold = 40;
    // Lọc lấy người (Class PERSON thường là class 0 hoặc có enum riêng)
    obj_runtime_param.object_class_filter.push_back(OBJECT_CLASS::PERSON);

    Objects objects;

    cout << ">>> ROBOT VISION STARTED (SDK v3 Compatible) <<<" << endl;

    while (true) {
        if (zed.grab() == ERROR_CODE::SUCCESS) {
            zed.retrieveObjects(objects, obj_runtime_param);

            if (objects.is_new) {
                for (auto& obj : objects.object_list) {
                    int id = obj.id;
                    float x = obj.position.x;
                    float y = obj.position.y;
                    float z = obj.position.z;
                    float distance = sqrt(x*x + y*y + z*z);

                    cout << "Phat hien Nguoi ID " << id << " | Cach: " << distance << "m" << endl;
                    
                    if (distance < 1.5) cout << "   -> HANH DONG: Xin chao!" << endl;
                }
            }
        }
    }

    zed.disableObjectDetection();
    zed.disablePositionalTracking();    
    zed.close();
    return 0;
}