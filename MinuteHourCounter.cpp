class MinuteHourCounter
{
public:
    // カウントを追加する
    void Count(int num_bytes);

    // 直近1分間のカウントを返す
    int MinuteCount();

    // 直近1時間のカウントを返す
    int HourCount();
}
