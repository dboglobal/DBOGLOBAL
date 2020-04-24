CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 130;
	title = 13002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "203;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 1;
			prelnk = "0;";
			nolnk = 255;
			rm = 0;
			yeslnk = 2;

			CDboTSCheckQItem
			{
				ct = 1;
				icnt = 1;
				iidx = 436;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 202;
			prelnk = "1;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
			CDboTSActObjState
			{
				mstate = 0;
				oidx = 402;
				osh_sh = 0;
				widx = 1;
				osh_uc = 1;
				taid = 2;
			}
		}
		CDboTSContGAct
		{
			cid = 202;
			elnk = 255;
			nextlnk = 203;
			prelnk = "2;";

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
				mstate = 0;
				oidx = 402;
				osh_sh = 1;
				widx = 1;
				osh_uc = 0;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 13007;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 25;
				minlvl = 17;
			}
			CDboTSClickObject
			{
				objidx = "402;";
				widx = 1;
			}
		}
	}
}

