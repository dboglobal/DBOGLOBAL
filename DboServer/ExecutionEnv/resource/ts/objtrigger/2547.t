CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 2547;
	title = 254702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 254;
			prelnk = "13;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = -1;
				osh_sh = 1;
				widx = -1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 14;
			prelnk = "12;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 30000;
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
				mstate = 255;
				oidx = -1;
				osh_sh = 0;
				widx = -1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 12;
			prelnk = "10;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1231;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 11;
			prelnk = "3;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 254718;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;14;";
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
			stdiag = 254707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSClickObject
			{
				objidx = "1225;1226;1227;1228;1229;1230;1231;1232;1233;1234;1235;1236;1237;1238;1239;1240;1241;1242;1243;1244;";
				widx = 1;
			}
			CDboTSCheckObjItem
			{
				tid = 2547;
			}
		}
	}
}

