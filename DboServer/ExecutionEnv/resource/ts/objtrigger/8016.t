CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 8016;
	title = 801602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 10;
			prelnk = "0;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 10;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 12;
				osh_sh = 255;
				widx = 300000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "10;";
			type = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 801607;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckAttachObj
			{
				objidx = "12;";
				widx = 300000;
			}
		}
	}
}

