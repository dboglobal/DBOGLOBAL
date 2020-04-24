CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 872;
	title = 87202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 1;
			prelnk = "12;";

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
			cid = 12;
			prelnk = "11;";
			nolnk = 254;
			rm = 0;
			yeslnk = 13;

			CDboTSCheckOPObject
			{
			}
		}
		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 12;
			prelnk = "0;";

			CDboTSActOPObject
			{
				apply = 1;
				taid = 1;
				tblidx = 10003;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 254;
			prelnk = "5;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 1;
				osh_sh = 1;
				widx = 3;
				osh_uc = 0;
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
				time = 60000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = 1;
				osh_sh = 0;
				widx = 3;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 87207;
			nolnk = 255;
			rm = 0;
			yeslnk = 11;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 99064;
			}
			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 872;
			}
			CDboTSCheckAttachObj
			{
				objidx = "1;";
				widx = 3;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "13;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActPCConv
			{
				taid = 4;
				tblidx = 87230;
			}
			CDboTSActItem
			{
				iidx0 = 99064;
				stype0 = 1;
				taid = 2;
				type = 1;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = 1;
				osh_sh = 1;
				widx = 3;
				osh_uc = 1;
				taid = 1;
			}
			CDboTSActSendSvrEvt
			{
				id = 188;
				tblidx = -1;
				stradius = 200;
				stype = 3;
				taid = 3;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 7000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "12;6;";
			type = 1;
		}
	}
}

