CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6212;
	title = 621202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 12;
			prelnk = "10;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 11;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1202;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 13;
			prelnk = "11;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = -1;
				osh_sh = 0;
				widx = -1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 254;
			prelnk = "12;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "13;2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 254;
			rm = 0;
			yeslnk = 3;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 10;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 621207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckObjItem
			{
				tid = 6212;
			}
			CDboTSClickObject
			{
				objidx = "46;47;48;49;50;51;52;53;54;55;";
				widx = 200000;
			}
		}
	}
}

