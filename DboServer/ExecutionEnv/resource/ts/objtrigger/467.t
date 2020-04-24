CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 467;
	title = 46702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 13;
			prelnk = "3;";

			CDboTSActSendSvrEvt
			{
				id = 13;
				tblidx = -1;
				stradius = 200;
				stype = 3;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;203;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 201;
			elnk = 255;
			nextlnk = 202;
			prelnk = "51;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = 478;
				osh_sh = 0;
				widx = 1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 202;
			elnk = 255;
			nextlnk = 203;
			prelnk = "201;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 60000;
			}
		}
		CDboTSContGAct
		{
			cid = 51;
			elnk = 255;
			nextlnk = 201;
			prelnk = "13;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 203;
			elnk = 255;
			nextlnk = 254;
			prelnk = "202;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = 478;
				osh_sh = 1;
				widx = 1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 51;
			prelnk = "4;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 46718;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 46707;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 467;
			}
			CDboTSClickObject
			{
				objidx = "478;";
				widx = 1;
			}
		}
	}
}

