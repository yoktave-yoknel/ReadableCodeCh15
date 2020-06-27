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

    list<Event> minute_events;
    list<Event> hour_events; // 直近1分間のイベントは含まれて「いない」

    int minute_count;
    int hour_count; // 直近1時間の「すべての」イベントをカウントしている

    // 古いイベントを見つけて削除して、hour_countとminute_countを減らす。
    void ShiftOldEvents(time_t now_secs) {
        const int minute_ago = now_secs - 60;
        const int hour_ago = now_secs - 3600;

        // 1分以上経過したイベントを'minute_events'から'hour_events'へと移動する。
        // (1時間以上経過した古いイベントは次のループで削除する)
        while (!minute_events.empty() && minute_events.front().time <= minute_ago) {
            hour_events.push_back(minute_events.front());

            minute_count -= minute_events.front().count;
            minute_events.pop_front();
        }

        // 1時間以上経過した古いイベントを'hour_events'から削除する
        while (!hour_events.empty() && hour_events.front().time <= hour_ago) {
            hour_count -= hour_events.front().count;
            hour_events.pop_front();
        }
    }

public:
    // 新しいデータ点を追加する (count >= 0)。
    // それから1分間は、MinuteCount()の返す値が+count分だけ増える。
    // それから1時間は、HourCount()の返す値が+count分だけ増える。
    void Add(int count) {
        const time_t now_secs = time();
        ShiftOldEvents(now_secs);

        // 1分間のリストに流し込む(1時間のリストはあとから)
        minute_events.push_back(Event(count, now_secs));

        minute_count += count;
        hour_count += count;

    }

    // 直近60秒間の累積カウントを返す。
    int MinuteCount() {
        ShiftOldEvents(time());
        return minute_count;
    }

    // 直近3600秒間の累積カウントを返す。
    int HourCount() {
        ShiftOldEvents(time());
        return hour_count;
    }
};
