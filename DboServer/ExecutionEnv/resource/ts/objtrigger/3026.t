CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3026;
	title = 302602;

	CNtlTSGroup
	{
		gid = 0;

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
			prelnk = "6;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 30000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "10;2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 9;
			prelnk = "5;";

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
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1551;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 302607;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckObjItem
			{
				tid = 3026;
			}
			CDboTSClickObject
			{
				objidx = "2080;2081;2082;2083;2084;2085;2086;2087;2088;2089;2090;2091;2092;2093;2094;";
				widx = 1;
			}
		}
	}
}

