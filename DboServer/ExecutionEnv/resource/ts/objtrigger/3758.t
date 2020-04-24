CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 3758;
	title = 375802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 20;
			prelnk = "8;8;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 22;
				osh_sh = 1;
				widx = 15;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 22;
				osh_sh = 1;
				widx = 15;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 9;
			nextlnk = 9;
			prelnk = "7;";

			CDboTSActObjWPS
			{
				sid = 3758;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActSendSvrEvt
			{
				id = 1205;
				tblidx = -1;
				stype = 2;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContGAct
		{
			cid = 20;
			elnk = 255;
			nextlnk = 201;
			prelnk = "9;";

			CDboTSActTWaitTS
			{
				taid = 2;
				time = 15000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "201;2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 201;
			elnk = 255;
			nextlnk = 254;
			prelnk = "20;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 22;
				osh_sh = 1;
				widx = 15;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "3;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
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
			nextlnk = 5;
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
			stdiag = 375807;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 3758;
			}
			CDboTSCheckAttachObj
			{
				objidx = "22;";
				widx = 15;
			}
		}
	}
}

