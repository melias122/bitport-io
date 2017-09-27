package bitport

import (
	"testing"
)

var jsonString = `{
    "status": "success",
    "data": [
        {
            "name": "My files",
            "code": null,
            "size": 6378455526,
            "files_count": null,
            "created_at": null,
            "folders": [
                {
                    "name": "Movies",
                    "code": "ELH6tNhEUK",
                    "size": 0,
                    "files_count": 0,
                    "created_at": {
                        "date": "2017-08-29 14:15:19.000000",
                        "timezone_type": 3,
                        "timezone": "UTC"
                    }
                },
                {
                    "name": "TV Shows",
                    "code": "TsNRcbgelS",
                    "size": 0,
                    "files_count": 0,
                    "created_at": {
                        "date": "2017-08-29 14:15:19.000000",
                        "timezone_type": 3,
                        "timezone": "UTC"
                    }
                },
                {
                    "name": "Music",
                    "code": "TFZdBEUAID",
                    "size": 0,
                    "files_count": 0,
                    "created_at": {
                        "date": "2017-08-29 14:15:19.000000",
                        "timezone_type": 3,
                        "timezone": "UTC"
                    }
                },
                {
                    "name": "Photos",
                    "code": "NVPjhhzYXA",
                    "size": 0,
                    "files_count": 0,
                    "created_at": {
                        "date": "2017-08-29 14:15:19.000000",
                        "timezone_type": 3,
                        "timezone": "UTC"
                    }
                },
                {
                    "name": "The.Fate.of.the.Furious.2017.BDRip.x264.CZ-TreZzoR",
                    "code": "pz8wsutm1b",
                    "size": 1290549103,
                    "files_count": 2,
                    "created_at": {
                        "date": "2017-08-29 16:32:48.000000",
                        "timezone_type": 3,
                        "timezone": "UTC"
                    }
                },
                {
                    "name": "Oblivion 2013 BRRiP AC3 XViD.cCZ-sC0rp",
                    "code": "4jo80rd2su",
                    "size": 1656118201,
                    "files_count": 4,
                    "created_at": {
                        "date": "2017-08-29 18:07:46.000000",
                        "timezone_type": 3,
                        "timezone": "UTC"
                    }
                },
                {
                    "name": "Despicable.Me.3.2017.HDRip.XviD.AC3-EVO.CZ",
                    "code": "rn9zu29d3r",
                    "size": 1272470298,
                    "files_count": 2,
                    "created_at": {
                        "date": "2017-09-02 15:55:06.000000",
                        "timezone_type": 3,
                        "timezone": "UTC"
                    }
                }
            ],
            "files": [
                {
                    "name": "Mumie se vrací.avi",
                    "crc32": 2706176767,
                    "created_at": {
                        "date": "2017-08-29 16:41:55.000000",
                        "timezone_type": 3,
                        "timezone": "UTC"
                    },
                    "code": "qlfm4l61om",
                    "parent_folder_code": null,
                    "size": 717106084,
                    "video": true,
                    "screenshots": {
                        "small": "https://static.bitport.io/a301a9e324fbd113_s01.jpg",
                        "medium": "https://static.bitport.io/a301a9e324fbd113_m01.jpg",
                        "big": "https://static.bitport.io/a301a9e324fbd113_l01.jpg"
                    },
                    "extension": "avi",
                    "type": "video",
                    "conversion_status": "unconverted"
                },
                {
                    "name": "Interstellar 2014 Cz dab..avi",
                    "crc32": 181549490,
                    "created_at": {
                        "date": "2017-08-29 16:52:14.000000",
                        "timezone_type": 3,
                        "timezone": "UTC"
                    },
                    "code": "90n616f3j5",
                    "parent_folder_code": null,
                    "size": 1442211840,
                    "video": true,
                    "screenshots": {
                        "small": "https://static.bitport.io/2bae31c4ff10add2_s01.jpg",
                        "medium": "https://static.bitport.io/2bae31c4ff10add2_m01.jpg",
                        "big": "https://static.bitport.io/2bae31c4ff10add2_l01.jpg"
                    },
                    "extension": "avi",
                    "type": "video",
                    "conversion_status": "unconverted"
                }
            ],
            "parent_folder": null
        }
    ],
    "errors": null
}`

var jsonRespError = `{
    "status": "error",
    "data": null,
    "errors": [
        {
            "message": "Method not found",
            "code": 404
        }
    ]
}`

func TestUnmarshal(t *testing.T) {
	//var resp respCloud
	//if err := json.Unmarshal([]byte(jsonString), &resp); err != nil {
	//	t.Fatal(err)
	//}
	//t.Logf("%+v\n", resp)
}
