CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 215;
	title = 21502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "203;";
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
				oidx = -1;
				osh_sh = 0;
				widx = -1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 51;
			elnk = 255;
			nextlnk = 201;
			prelnk = "0;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 5000;
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
				time = 30000;
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
				oidx = -1;
				osh_sh = 1;
				widx = -1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 21507;
			nolnk = 255;
			rm = 0;
			yeslnk = 51;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 462;
			}
			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 215;
			}
			CDboTSClickObject
			{
				objidx = "436;437;438;";
				widx = 1;
			}
		}
	}
}

