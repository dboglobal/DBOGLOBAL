CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 1220;
	title = 122002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 254;
			prelnk = "4;";

			CDboTSActObjState
			{
				mstate = 0;
				oidx = 657;
				osh_sh = 255;
				widx = 1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 15000;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 657;
				osh_sh = 255;
				widx = 1;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 122007;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckProgQuest
			{
				not = 0;
				qid = 1220;
			}
			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 797;
			}
			CDboTSClickObject
			{
				objidx = "657;";
				widx = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActPCConv
			{
				taid = 2;
				tblidx = 122018;
			}
			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 797;
				taid = 1;
				type = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "5;";
			type = 1;
		}
	}
}

