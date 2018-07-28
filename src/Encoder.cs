using System;
using System.Collections.Generic;
using System.Linq;
using System.Text.RegularExpressions;
    
public class Entry {
    private string key;
    private string val;

    public Entry(string key, string val) {
        Key = key;
        Value = val;
    }

    public string Key {
        get { return key; }
        set { key = value; }
    }

    public string Value {
        get { return val; }
        set { val = value; }
    }
}
    
public class Record {
    private List<Entry> entries;
        
    public Record(List<Entry> entries) {
        this.entries = entries;
    }
        
    public List<Entry> Entries {
        get { return entries; }
    }
}
    
public class Encoder {
    const char ENTRY_DELIMITER = '#';
    const char ENTRY_KEY_EQUAL = '=';
    private static Regex regex = new Regex("^[a-zA-Z0-9]*$");

    private static Boolean IsValid(string str) {
        return (str.Length != 0 && regex.IsMatch(str));
    }

    private static string EncodeEntry(Entry e) {
        if (!IsValid(e.Key) || !IsValid(e.Value)) {
            throw new System.InvalidOperationException("Entry is malformed");
        }
        return e.Key + ENTRY_KEY_EQUAL + e.Value;
    }
        
    public static string Encode(Record rec) {
        var entries = rec.Entries.Select(x => EncodeEntry(x)).ToList();
        return String.Join("" + ENTRY_DELIMITER, entries.ToArray());
    }

    public static Entry DecodeEntry(string str) {
        string[] tokens = str.Split(ENTRY_KEY_EQUAL);
        if (tokens.Length != 2 || !IsValid(tokens[0]) || !IsValid(tokens[1])) {
            throw new System.InvalidOperationException("Entry is malformed");
        }
        return new Entry(tokens[0], tokens[1]);
    }

    public static Record Decode(string str) {
        string[] tokens = str.Split(ENTRY_DELIMITER);
        List<Entry> entries = new List<Entry>();
        foreach (string token in tokens) {
            entries.Add(DecodeEntry(token));
        }
        return new Record(entries);
    }
}