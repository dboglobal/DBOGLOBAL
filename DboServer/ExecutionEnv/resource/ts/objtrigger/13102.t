CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 13102;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 4;
			prelnk = "3;";
			nolnk = 5;
			rm = 0;
			yeslnk = 6;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 98029;
			}
		}
		CDboTSContEnd
		{
			cid = 211;
			prelnk = "7;2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "4;5;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 211;
			prelnk = "6;";

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
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActItem
			{
				iidx0 = 98029;
				stype0 = 1;
				taid = 1;
				type = 0;
			}
		}
		CDboTSContGCond
		{
			cid = 2;
			prelnk = "1;";
			nolnk = 211;
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
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 13108;
			}
			CDboTSClickObject
			{
				objidx = "6;";
				widx = 211000;
			}
		}
	}
}

