CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 2958;
	title = 295802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 295809;
				ctype = 1;
				idx = 4651211;
				taid = 2;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				eitype = 5;
				idx0 = 295801;
				cnt0 = 1;
				ectype = -1;
				etype = 0;
				qtidx = 295815;
				taid = 3;
				esctype = 0;
			}
			CDboTSActRegQInfo
			{
				cont = 295808;
				area = 295801;
				goal = 295804;
				m0fz = "-4443.069824";
				m0widx = 1;
				scitem = -1;
				sort = 295805;
				stype = 2;
				m0fy = "0.000000";
				m0pat = 2;
				grade = 132203;
				m0fx = "2873.600098";
				m0ttip = 295815;
				rwd = 100;
				taid = 1;
				title = 295802;
				gtype = 2;
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
				eitype = 5;
				idx0 = -1;
				cnt0 = 0;
				ectype = -1;
				etype = 1;
				qtidx = -1;
				taid = 1;
				esctype = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 295814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 295801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4651211;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "101;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
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
				itype = 5;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;252;0;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 295808;
			gtype = 2;
			oklnk = 2;
			area = 295801;
			goal = 295804;
			sort = 295805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 295802;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 295807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 56;
			}
			CDboTSCheckClrQst
			{
				and = "2951;";
				flink = 1;
				flinknextqid = "2959;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4651211;";
			}
		}
	}
}

