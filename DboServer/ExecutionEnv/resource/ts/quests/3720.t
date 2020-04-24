CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 3720;
	title = 372002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 254;
			prelnk = "100;";

			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = -1;
				cnt0 = 0;
				cnt2 = 0;
				ectype = -1;
				etype = 1;
				idx1 = -1;
				taid = 1;
				esctype = 0;
				idx2 = -1;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;2;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 372008;
			gtype = 1;
			oklnk = 2;
			area = 372001;
			goal = 372004;
			sort = 372005;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 372002;
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSCheckSToCEvt
			{
				itype = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 372007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 60;
			}
			CDboTSClickNPC
			{
				npcidx = "7074102;";
			}
			CDboTSCheckClrQst
			{
				and = "3719;";
				flink = 0;
				flinknextqid = "3721;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 372014;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 372001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7072201;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 372009;
				ctype = 1;
				idx = 7074102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 372008;
				m3widx = 15;
				m1fy = "-87.330002";
				sort = 372005;
				m2widx = 15;
				grade = -1;
				m1pat = 2;
				m1widx = 15;
				m3fx = "-2735.350098";
				rwd = 100;
				taid = 1;
				gtype = 1;
				m3fy = "-87.330002";
				area = 372001;
				goal = 372004;
				m0fz = "-5512.669922";
				m0widx = 15;
				m1ttip = 372015;
				m2fy = "-85.760002";
				m2pat = 2;
				m3fz = "-5204.609863";
				m3ttip = 372016;
				scitem = -1;
				stype = 2;
				m0fy = "-85.760002";
				m0pat = 2;
				m1fz = "-5204.609863";
				m2fx = "-2655.639893";
				m2ttip = 372016;
				m0fx = "-2655.639893";
				m0ttip = 372015;
				m3pat = 2;
				title = 372002;
				m1fx = "-2735.350098";
				m2fz = "-5512.669922";
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 40;
				eitype = 0;
				idx0 = 19841207;
				cnt0 = 40;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 19843101;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
		}
	}
}

