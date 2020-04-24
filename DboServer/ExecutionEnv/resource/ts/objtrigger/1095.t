CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 1095;
	title = 109502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 254;
			prelnk = "2;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 41;
				osh_sh = 255;
				widx = 4;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 255;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckObjState
			{
				mstate = 3;
				oidx = 11;
				osh = 255;
				widx = 4;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "3;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActTWaitTS
			{
				taid = 2;
				time = 60000;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = 41;
				osh_sh = 255;
				widx = 4;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 109507;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 1092;
			}
			CDboTSClickObject
			{
				objidx = "11;";
				widx = 4;
			}
		}
	}
}

