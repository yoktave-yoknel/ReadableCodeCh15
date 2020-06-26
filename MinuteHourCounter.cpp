// 直近1分間および直近1時間の累積カウントを記録する。
// 例えば、帯域幅の使用状況を確認するのに使える。
class MinuteHourCounter
{
    struct Event
    {
        Event(int count, time_t time) : count(count), time(time) {}
        int count;
        time_t time;
    };

    list<Event> events;
    
public:
    // 新しいデータ点を追加する (count >= 0)。
    // それから1分間は、MinuteCount()の返す値が+count分だけ増える。
    // それから1時間は、HourCount()の返す値が+count分だけ増える。
    void Add(int count) {
        events.push_back(Event(count, time()));
    };

    // 直近60秒間の累積カウントを返す。
    int MinuteCount() {
        int count = 0;
        const time_t now_secs = time();
        for (list<Event>::reverse_iterator i = events.rbegin();
             i != events.rend() && i->time > now_secs - 60; i++) {
            count += i->count;
        }
        return count;

    };

    // 直近3600秒間の累積カウントを返す。
    int HourCount() {
        int count = 0;
        const time_t now_secs = time();
        for (list<Event>::reverse_iterator i = events.rbegin();
             i != events.rend() && i->time > now_secs - 3600; i++) {
            count += i->count;
        }
        return count;

    };
}
