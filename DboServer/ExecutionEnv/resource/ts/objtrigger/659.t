CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 659;
	title = 65902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 5000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "3;";

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
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 15000;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActObjState
			{
				mstate = 255;
				oidx = -1;
				osh_sh = 1;
				widx = -1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 65907;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 534;
			}
			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 659;
			}
			CDboTSClickObject
			{
				objidx = "450;451;452;";
				widx = 1;
			}
		}
	}
}

