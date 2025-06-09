#include "figureInfo.h"

const std::vector<DataObject>* bearingDataObjects = nullptr;
const std::map<DataObject, std::vector<DataQuantity>>* dataObjectToDataQuantity = nullptr;
const std::map<DataObject, std::map<DataQuantity, DataNameAndFileName>>* dataTypeToFileName = nullptr;
std::map<DataObject, int> dataObjectEntityNum = {
    {DataObject::Unknow, 1},
    {DataObject::Num, 1},
    {DataObject::Time, 1},
    {DataObject::InnerRace, 1},
    {DataObject::OuterRace, 1},
    {DataObject::Roller, 1},//在程序运行过程中修改。
    {DataObject::RollerCage, 1},
    {DataObject::RollerInnerRace, 1},
    {DataObject::RollerOuterRace, 1},
    {DataObject::Cage, 1},
    {DataObject::CageRace, 1},
};

BearingType presentBearingType = BearingType::Null;

const std::map<DataObject, QString> dataObjectToText = {
    {DataObject::Unknow, "未知"},
    {DataObject::Num, "累加计数"},
    {DataObject::Time, "时间"},
    {DataObject::InnerRace, "轴承内圈"},
    {DataObject::OuterRace, "轴承外圈"},
    {DataObject::Roller, "滚动体"},
    {DataObject::RollerCage, "滚动体-套圈"},
    {DataObject::RollerInnerRace, "滚动体-内圈"},
    {DataObject::RollerOuterRace, "滚动体-外圈"},
    {DataObject::Cage, "保持架"},
    {DataObject::CageRace, "保持架-引导套圈"},
};

const std::vector<DataObject> cylindricalRollerDataObjects = {
    DataObject::Unknow,
    DataObject::Num,
    DataObject::Time,
    DataObject::InnerRace,
    DataObject::OuterRace,
    DataObject::Roller,
    DataObject::RollerCage,
    DataObject::RollerInnerRace,
    DataObject::RollerOuterRace,
    DataObject::Cage,
    DataObject::CageRace,
};
const std::map<DataObject, std::map<DataQuantity, DataNameAndFileName>> cylindricalRollerDataTypeToFileName = {
   {DataObject::Unknow, {{DataQuantity::Null, {"未知", "", {""}}}}},
   {DataObject::Num, {{DataQuantity::Null, {"累加计数", "", {""}}}}},
   {DataObject::Time, {{DataQuantity::Null, {"时间(s)", ".txt", {""}}}}},
   {DataObject::InnerRace, {{DataQuantity::TranslationP, {"质心平动位移(mm)", ".txt", cartesianCoordinate}},
                            {DataQuantity::TranslationV, {"质心平动速度(mm/s)", ".txt", cartesianCoordinate}},
                            {DataQuantity::TranslationA, {"质心平动加速度(mm/s²)", ".txt", cartesianCoordinate}},

                           {DataQuantity::ThermalDisp, {"热膨胀量(mm)", ".txt", {"column:0"}}}}},

   {DataObject::OuterRace, {{DataQuantity::TranslationP, {"平动位移(mm)", ".txt", patialCartesianCoordinate}},
                            {DataQuantity::TranslationV, {"平动速度(mm/s)", ".txt", patialCartesianCoordinate}},
                            {DataQuantity::TranslationA, {"平动加速度(mm/s²)", ".txt", patialCartesianCoordinate}},

                           {DataQuantity::ThermalDisp, {"热膨胀量(mm)", ".txt", { "column:1"}}}}},

   {DataObject::Roller, {{DataQuantity::TranslationP, {"平动位移(mm)", ".txt", cylindricalCoordinate}},
                            {DataQuantity::TranslationV, {"平动速度(mm/s)", ".txt", cylindricalCoordinate}},
                            {DataQuantity::TranslationA, {"平动加速度(mm/s²)", ".txt", cylindricalCoordinate}},

                           {DataQuantity::RotationP, {"自转角位移(rad)", ".txt", angleCoordinate}},
                            {DataQuantity::RotationV, {"自转角速度(rad/s)", ".txt", angleCoordinate}},
                            {DataQuantity::RotationA, {"自转角加速度(rad/s²)", ".txt", angleCoordinate}},

                           {DataQuantity::ThermalDisp, {"热膨胀量(mm)", ".txt", {"column:2" }}}}},

   {DataObject::RollerCage, {{DataQuantity::PocketFrontQ, {"接触坐标系中兜孔前端作用载荷(N)", ".txt", cartesianCoordinate}},
                           {DataQuantity::PocketBackQ, {"接触坐标系中兜孔后端作用载荷(N)", ".txt", cartesianCoordinate}}}},

   {DataObject::RollerInnerRace, {{DataQuantity::RaceContactTemperature, {"接触点温度(K)", ".txt", {"column:0" }}},
                                   {DataQuantity::QBC, {"接触坐标系中的作用载荷(N)", ".txt", cartesianCoordinate}},
                                    {DataQuantity::MBb, {"滚动体定体坐标系中滚动体受到来自内圈作用的力矩(N·mm)", ".txt", cartesianCoordinate}},
                                    {DataQuantity::Delta, {"滚动体-滚道之间的几何趋近量(mm)", ".txt", { ""}}},
                                   {DataQuantity::ContactV, {"接触坐标系中滚道接触点的速度(mm/s)", ".txt", cartesianCoordinate}},
                                    {DataQuantity::ContactQ, {"滚道之间的接触载荷(N)", ".txt", {""}}},
                                    {DataQuantity::ContactR, {"滚道接触短半轴长度(mm)", ".txt", {"" }}},
                                   {DataQuantity::ContactPMax, {"滚道接触正压力最大值(MPa)", ".txt", {"" }}},

                                   {DataQuantity::RollerInnerContactInnerV, {"接触坐标系中滚道接触点内滚道速度(mm/s)", ".txt", cartesianCoordinate}}}},

   {DataObject::RollerOuterRace, {{DataQuantity::RaceContactTemperature, {"接触点温度(K)", ".txt", {"column:1" }}},
                                   {DataQuantity::QBC, {"接触坐标系中的作用载荷(N)", ".txt", cartesianCoordinate}},
                                    {DataQuantity::MBb, {"滚动体定体坐标系中滚动体受到来自外圈作用的力矩(N·mm)", ".txt", cartesianCoordinate}},
                                    {DataQuantity::Delta, {"滚动体-滚道之间的几何趋近量(mm)", ".txt", {"" }}},
                                   {DataQuantity::ContactV, {"接触坐标系中滚动体-滚道接触点的速度(mm/s)", ".txt", cartesianCoordinate}},
                                    {DataQuantity::ContactQ, {"滚动体-滚道之间的接触载荷(N)", ".txt", {""}}},
                                    {DataQuantity::ContactR, {"滚动体-滚道接触短半轴长度(mm)", ".txt", { ""}}},
                                   {DataQuantity::ContactPMax, {"滚动体-滚道接触正压力最大值(MPa)", ".txt", {"" }}}}},

   {DataObject::Cage, {{DataQuantity::TranslationP, {"质心平动位移(mm)", ".txt", cartesianCoordinate}},
                       {DataQuantity::TranslationV, {"质心平动速度(mm/s)", ".txt", cartesianCoordinate}},

                       {DataQuantity::RotationP, {"角位移(rad)", ".txt", angleCoordinate}},
                        {DataQuantity::RotationV, {"角速度(rad/s)", ".txt", angleCoordinate}}}},

   {DataObject::CageRace, {{DataQuantity::ContactQ, {"接触载荷(N)", ".txt", cartesianCoordinate}}}},
};
const std::map<DataObject, std::vector<DataQuantity>> cylindricalRollerDataObjectToDataQuantity = {
    {DataObject::Unknow, {DataQuantity::Null,}},
    {DataObject::Num, {DataQuantity::Null,}},
    {DataObject::Time, {DataQuantity::Null,}},
    {DataObject::InnerRace, {DataQuantity::TranslationP, DataQuantity::TranslationV, DataQuantity::TranslationA,
                            DataQuantity::ThermalDisp}},
    {DataObject::OuterRace, {DataQuantity::TranslationP, DataQuantity::TranslationV, DataQuantity::TranslationA,
                            DataQuantity::ThermalDisp}},
    {DataObject::Roller, {DataQuantity::TranslationP, DataQuantity::TranslationV, DataQuantity::TranslationA,
                            DataQuantity::RotationP, DataQuantity::RotationV, DataQuantity::RotationA,
                            DataQuantity::ThermalDisp}},
    {DataObject::RollerCage, {DataQuantity::PocketFrontQ, DataQuantity::PocketBackQ}},
    {DataObject::RollerInnerRace, {DataQuantity::RaceContactTemperature,
                                    DataQuantity::QBC, DataQuantity::MBb, DataQuantity::Delta,
                                    DataQuantity::ContactV, DataQuantity::ContactQ, DataQuantity::ContactR,
                                    DataQuantity::ContactPMax, DataQuantity::RollerInnerContactInnerV}},
    {DataObject::RollerOuterRace, {DataQuantity::RaceContactTemperature,
                                    DataQuantity::QBC, DataQuantity::MBb, DataQuantity::Delta,
                                    DataQuantity::ContactV, DataQuantity::ContactQ, DataQuantity::ContactR,
                                    DataQuantity::ContactPMax}},
    {DataObject::Cage, {DataQuantity::TranslationP,DataQuantity::TranslationV,
                        DataQuantity::RotationP, DataQuantity::RotationV}},
    {DataObject::CageRace, {DataQuantity::ContactQ}},
};

const std::vector<DataObject> ballRollerDataObjects = {
    DataObject::Unknow,
    DataObject::Num,
    DataObject::Time,
    DataObject::InnerRace,
    DataObject::OuterRace,
    DataObject::Roller,
    DataObject::RollerCage,
    DataObject::RollerInnerRace,
    DataObject::RollerOuterRace,
    DataObject::Cage,
    DataObject::CageRace,
};
const std::map<DataObject, std::map<DataQuantity, DataNameAndFileName>> ballRollerDataTypeToFileName = {
    {DataObject::Unknow, {{DataQuantity::Null, {"未知", "", {""}}}}},
    {DataObject::Num, {{DataQuantity::Null, {"累加计数", "", {""}}}}},
    {DataObject::Time, {{DataQuantity::Null, {"时间(s)", ".txt", {""}}}}},
    {DataObject::InnerRace, {{DataQuantity::TranslationP, {"质心平动位移(m)", ".txt", cartesianCoordinate}},
                             {DataQuantity::TranslationV, {"质心平动速度(m/s)", ".txt", cartesianCoordinate}},
                             {DataQuantity::TranslationA, {"内圈加速度(m/s²)", ".txt", cartesianCoordinate}},

                            {DataQuantity::ThermalDisp, {"热膨胀量(mm)", ".txt", {"column:0"}}},
                            { DataQuantity::HeatGeneration, {"生热率(W)", ".txt", { "column:0"}} },
}},
    {DataObject::OuterRace, {{DataQuantity::TranslationP, {"外圈(基座)位移(m)", ".txt", patialCartesianCoordinate}},
                             {DataQuantity::TranslationV, {"外圈(基座)速度(m/s)", ".txt", patialCartesianCoordinate}},
                             {DataQuantity::TranslationA, {"外圈(基座)加速度(m/s²)", ".txt", patialCartesianCoordinate}},

                            {DataQuantity::ThermalDisp, {"热膨胀量(mm)",  "thermaldisp.txt", { "column:1"}}},
                            {DataQuantity::HeatGeneration, {"生热率(W)",  "heatgeneration.txt", { "column:1"}}},
                            {DataQuantity::Deformation, {"柔性变形量(m)", ".txt", cylindricalCoordinate}}
}},
    {DataObject::Roller, {{DataQuantity::TranslationP, {"平动位移(mm)", ".txt", cylindricalCoordinate}},
                             {DataQuantity::TranslationV, {"平动速度(mm/s)", ".txt", cylindricalCoordinate}},
                             {DataQuantity::TranslationA, {"平动加速度(mm/s²)", ".txt", cylindricalCoordinate}},

                             {DataQuantity::RotationV, {"滚球方位系中描述的]自转角速度(rad/s)", ".txt", angleCoordinate}},
                            }},

    {DataObject::RollerCage, {{DataQuantity::ContactQ, {"滚球对保持架作用力(N)", ".txt", cartesianCoordinate}},
                            {DataQuantity::PocketQ, {"滚球方位系中描述的滚球受保持架兜孔作用力(N)", ".txt", cartesianCoordinate}},
                            {DataQuantity::TranslationV, {"滚球方位系中描述的滚球和兜孔之间滑动速度(m/s)", ".txt", cartesianCoordinate}},

}},

    {DataObject::RollerInnerRace, {{DataQuantity::RaceContactTemperature, {"接触点温度(K)", ".txt", {"column:0" }}},
                                     {DataQuantity::ContactQ, {"滚道之间的接触载荷(N)", ".txt", {""}}},
                                    {DataQuantity::ContactPMax, {"滚道接触正压力最大值(Pa)", ".txt", {"" }}},
                                    {DataQuantity::ContactAngle, {"接触角(rad)", ".txt", {""}}},

                                    {DataQuantity::ThermalDisp, {"热膨胀量(mm)",  "thermaldisp.txt", { "column:2"}}},
}},

    {DataObject::RollerOuterRace, {{DataQuantity::RaceContactTemperature, {"接触点温度(K)", ".txt", {"column:1" }}},
                                     {DataQuantity::ContactQ, {"滚动体-滚道之间的接触载荷(N)", ".txt", {""}}},
                                    {DataQuantity::ContactPMax, {"滚动体-滚道接触正压力最大值(Pa)", ".txt", {"" }}},
                                    {DataQuantity::ContactAngle, {"接触角(rad)", ".txt", {""}}},

                                     { DataQuantity::ThermalDisp, {"热膨胀量(mm)",  "thermaldisp.txt", { "column:3"}} }
                                    }},

    {DataObject::Cage, {{DataQuantity::TranslationP, {"质心平动位移(m)", ".txt", cartesianCoordinate}},
                        {DataQuantity::TranslationV, {"质心平动速度(m/s)", ".txt", cartesianCoordinate}},
                        {DataQuantity::TranslationA, {"质心平动加速度(m/s²)", ".txt", cartesianCoordinate}},

                        {DataQuantity::RotationP, {"相对惯性系中心的旋转角度(rad)", ".txt", {""}}},
                         {DataQuantity::RotationV, {"相对惯性系中心的旋转角速度(rad/s)", ".txt", {""}}},
                         {DataQuantity::RotationA, {"相对惯性系中心的旋转角加速度(rad/s²)", ".txt", {""}}},
                         {DataQuantity::CentrifugalForce, {"离心力(N)", ".txt", {""}}},


}},

    {DataObject::CageRace, {{DataQuantity::ContactQ, {"接触载荷(N)", ".txt", cartesianCoordinate}}

}},
};
const std::map<DataObject, std::vector<DataQuantity>> ballRollerDataObjectToDataQuantity = {
    {DataObject::Unknow, {DataQuantity::Null,}},
    {DataObject::Num, {DataQuantity::Null,}},
    {DataObject::Time, {DataQuantity::Null,}},
    {DataObject::InnerRace, {DataQuantity::TranslationP,DataQuantity::TranslationV,DataQuantity::TranslationA,
                                DataQuantity::ThermalDisp,DataQuantity::HeatGeneration,}},
    {DataObject::OuterRace, {DataQuantity::TranslationP,DataQuantity::TranslationV,DataQuantity::TranslationA,
                            DataQuantity::ThermalDisp,DataQuantity::HeatGeneration,DataQuantity::Deformation}},
    {DataObject::Roller, {DataQuantity::TranslationP,DataQuantity::TranslationV,DataQuantity::TranslationA,
                            DataQuantity::RotationV,}},
    {DataObject::RollerCage, {DataQuantity::ContactQ,
                            DataQuantity::PocketQ,
                            DataQuantity::TranslationV,}},
    {DataObject::RollerInnerRace, {DataQuantity::RaceContactTemperature,
                                   DataQuantity::ContactQ,
                                    DataQuantity::ContactPMax,
                                    DataQuantity::ContactAngle,
                                    DataQuantity::ThermalDisp, }},
    {DataObject::RollerOuterRace, {DataQuantity::RaceContactTemperature,
                                     DataQuantity::ContactQ,
                                    DataQuantity::ContactPMax,
                                    DataQuantity::ContactAngle,
                                    DataQuantity::ThermalDisp, }},
    {DataObject::Cage, {DataQuantity::TranslationP,DataQuantity::TranslationV,DataQuantity::TranslationA,
                        DataQuantity::RotationP,DataQuantity::RotationV,DataQuantity::RotationA,
                         DataQuantity::CentrifugalForce,}},
    {DataObject::CageRace, {DataQuantity::ContactQ,}},
};