CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 228;
	title = 22802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 22807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 6;
			}
			CDboTSClickNPC
			{
				npcidx = "4511301;";
			}
			CDboTSCheckClrQst
			{
				and = "227;";
				flink = 1;
				flinknextqid = "231;";
				not = 0;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
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
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 22814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 22801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511301;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 22809;
				ctype = 1;
				idx = 4511301;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 0;
				eitype = 0;
				idx0 = 1813100;
				cnt0 = 3;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = -1;
				taid = 3;
				esctype = 0;
				idx2 = -1;
			}
			CDboTSActRegQInfo
			{
				cont = 22808;
				m1fx = "4455.810059";
				m1fy = "0.000000";
				sort = 22805;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "-2192.179932";
				grade = 132203;
				m0fx = "4228.569824";
				m0ttip = 22815;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 22802;
				gtype = 1;
				area = 22801;
				goal = 22804;
				m0fz = "-2225.389893";
				m0widx = 1;
				m1ttip = 22815;
				scitem = -1;
				stype = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 22808;
			gtype = 1;
			oklnk = 2;
			area = 22801;
			goal = 22804;
			sort = 22805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 22802;
		}
	}
}

