CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 2;
	title = 202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "4;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 292519;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 201;
			prelnk = "5;";

			CDboTSActSendSvrEvt
			{
				id = 727;
				tblidx = 2925;
				stype = 2;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContSwitch
		{
			bid1 = 12;
			cid = 4;
			cid1 = 12;
			prelnk = "3;";
			bid0 = 5;
			cid0 = 5;

			CDboTSActSWProbSF
			{
				fbi = 5;
				prob = "0.900000";
				sbi = 12;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckObjItem
			{
				tid = 2925;
			}
			CDboTSClickObject
			{
				objidx = "1965;";
				widx = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActOPObject
			{
				apply = 0;
				taid = 1;
				tblidx = -1;
				time = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 13;
			elnk = 255;
			nextlnk = 201;
			prelnk = "12;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 292518;
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
		CDboTSContGAct
		{
			cid = 201;
			elnk = 255;
			nextlnk = 202;
			prelnk = "6;13;";

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
		CDboTSContGAct
		{
			cid = 202;
			elnk = 255;
			nextlnk = 203;
			prelnk = "201;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 60000;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "203;2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 12;
			elnk = 255;
			nextlnk = 13;
			prelnk = "4;";

			CDboTSActQItem
			{
				icnt0 = 1;
				iprob0 = "1.000000";
				iidx0 = 1523;
				taid = 1;
				type = 0;
			}
		}
	}
}

