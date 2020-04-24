CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 8025;
	title = 802502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 14;
			prelnk = "12;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGCond
		{
			cid = 11;
			prelnk = "0;";
			nolnk = 254;
			rm = 0;
			yeslnk = 12;

			CDboTSCheckObjState
			{
				mstate = 1;
				oidx = 12;
				osh = 255;
				widx = 310000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "11;21;22;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 22;
			elnk = 255;
			nextlnk = 254;
			prelnk = "17;";

			CDboTSActObjWPS
			{
				sid = 26009;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 21;
			elnk = 255;
			nextlnk = 254;
			prelnk = "17;";

			CDboTSActObjWPS
			{
				sid = 26008;
				taid = 1;
			}
		}
		CDboTSContSwitch
		{
			bid1 = 1;
			cid = 17;
			cid1 = 21;
			prelnk = "16;";
			bid0 = 2;
			cid0 = 22;

			CDboTSActSWProbSF
			{
				fbi = 2;
				prob = "0.100000";
				sbi = 1;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 16;
			elnk = 255;
			nextlnk = 17;
			prelnk = "15;";

			CDboTSActTWaitTS
			{
				taid = 2;
				time = 1000;
			}
			CDboTSActObjState
			{
				mstate = 0;
				oidx = 12;
				osh_sh = 0;
				widx = 310000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 15;
			elnk = 255;
			nextlnk = 16;
			prelnk = "14;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 14;
			elnk = 255;
			nextlnk = 15;
			prelnk = "13;";

			CDboTSActObjState
			{
				mstate = 2;
				oidx = 12;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 255;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 13;
			prelnk = "11;";

			CDboTSActObjState
			{
				mstate = 1;
				oidx = 11;
				osh_sh = 255;
				widx = 310000;
				osh_uc = 1;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 802507;
			nolnk = 255;
			rm = 0;
			yeslnk = 11;

			CDboTSClickObject
			{
				objidx = "11;";
				widx = 310000;
			}
		}
	}
}

