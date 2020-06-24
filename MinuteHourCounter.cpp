class MinuteHourCounter
{
public:
    // 新しいデータ点を追加する (count >= 0)。
    // それから1分間は、MinuteCount()の返す値が+count分だけ増える。
    // それから1時間は、HourCount()の返す値が+count分だけ増える。
    void Add(int count);

    // 直近1分間のカウントを返す
    int MinuteCount();

    // 直近1時間のカウントを返す
    int HourCount();
}
