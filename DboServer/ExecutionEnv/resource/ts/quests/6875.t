CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 6875;
	title = 687502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 253;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 687509;
				ctype = 1;
				idx = 4151114;
				taid = 3;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 7212101;
				cnt0 = 20;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 2;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 687508;
				m1fx = "6570.700195";
				m1fy = "0.000000";
				sort = 687505;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2265.270020";
				grade = -1;
				m0fx = "6369.069824";
				m0ttip = 687515;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 687502;
				gtype = 1;
				area = 687501;
				goal = 687504;
				m0fz = "-2407.689941";
				m0widx = 1;
				m1ttip = 687515;
				scitem = -1;
				stype = 64;
			}
		}
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 687508;
			gtype = 1;
			oklnk = 2;
			area = 687501;
			goal = 687504;
			sort = 687505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 687502;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 687507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 15;
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				flinknextqid = "6878;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4151114;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "4;";
			type = 1;
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
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "2;1;0;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 687514;
			nextlnk = 4;
			rwdexp = 0;
			rwdtbl = 687501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4151114;";
			}
		}
	}
}

