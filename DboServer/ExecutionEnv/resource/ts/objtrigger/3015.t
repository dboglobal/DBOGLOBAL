CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3015;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 9;
			prelnk = "6;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 301518;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 254;
			prelnk = "9;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = -1;
				osh_sh = 1;
				widx = -1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 10;
			prelnk = "13;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 30000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "10;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 13;
			prelnk = "5;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = -1;
				osh_sh = 1;
				widx = -1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "0;";

			CDboTSActCustomEvt
			{
				ceid = 3015;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckCustomEvent
			{
				qid = 3015;
			}
			CDboTSCheckAttachObj
			{
				objidx = "2063;2064;2065;2066;2067;2068;2069;2070;2071;2072;2073;2074;2075;2076;2077;2078;2079;";
				widx = 1;
			}
		}
	}
}

