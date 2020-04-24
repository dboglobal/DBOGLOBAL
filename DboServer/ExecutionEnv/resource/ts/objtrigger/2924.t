CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 2924;
	title = 292402;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 9;
			elnk = 255;
			nextlnk = 201;
			prelnk = "8;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 8;
			elnk = 255;
			nextlnk = 9;
			prelnk = "7;";

			CDboTSActObjWPS
			{
				sid = 2924;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 7;
			prelnk = "5;";

			CDboTSActObjState
			{
				mstate = 3;
				oidx = 2121;
				osh_sh = 1;
				widx = 1;
				osh_uc = 1;
				taid = 2;
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 292407;
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 2924;
			}
			CDboTSCheckAttachObj
			{
				objidx = "2121;";
				widx = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 202;
			elnk = 255;
			nextlnk = 254;
			prelnk = "201;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 7;
			elnk = 255;
			nextlnk = 8;
			prelnk = "6;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 201;
			elnk = 255;
			nextlnk = 202;
			prelnk = "9;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 2121;
				osh_sh = 255;
				widx = 1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "202;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "0;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 2121;
				osh_sh = 1;
				widx = 1;
				osh_uc = 1;
				taid = 1;
			}
		}
	}
}

