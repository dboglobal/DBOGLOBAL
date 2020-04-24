CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 1075;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 254;
			prelnk = "2;2;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 970;
				osh_sh = 255;
				widx = 1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 3;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActObjWPS
			{
				sid = 1075;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 970;
				osh_sh = 255;
				widx = 1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "3;";
			type = 1;
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
				qid = 1075;
			}
			CDboTSClickObject
			{
				objidx = "970;";
				widx = 1;
			}
		}
	}
}

