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

    int CountSince(time_t cutoff) {
        int count = 0;
        for (list<Event>::reverse_iterator rit = events.rbegin();
             rit != events.rend(); ++rit) {
            if (rit->time <= cutoff) {
                break;
            }
            count += rit->count;

        }
        return count;
    }   
    
public:
    // 新しいデータ点を追加する (count >= 0)。
    // それから1分間は、MinuteCount()の返す値が+count分だけ増える。
    // それから1時間は、HourCount()の返す値が+count分だけ増える。
    void Add(int count) {
        events.push_back(Event(count, time()));
    }

    // 直近60秒間の累積カウントを返す。
    int MinuteCount() {
        return CountSince(time() - 60);
    }

    // 直近3600秒間の累積カウントを返す。
    int HourCount() {
        return CountSince(time() - 3600);
    }
};
