CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 807;
	title = 80702;

	CNtlTSGroup
	{
		gid = 0;

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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 80708;
			gtype = 2;
			oklnk = 2;
			area = 80701;
			goal = 80704;
			sort = 80705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 80702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 80714;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 80701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1351209;";
			}
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
			stdiag = 80707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 27;
			}
			CDboTSCheckClrQst
			{
				and = "806;";
				flink = 1;
				flinknextqid = "808;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "1351209;";
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
				conv = 80709;
				ctype = 1;
				idx = 1351209;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 80708;
				m1fx = "5104.339844";
				m1fy = "0.000000";
				sort = 80705;
				m0fy = "0.000000";
				m0pat = 2;
				m1fz = "1132.839966";
				grade = 132203;
				m0fx = "5104.339844";
				m0ttip = 80715;
				m1pat = 2;
				m1widx = 1;
				rwd = 100;
				taid = 1;
				title = 80702;
				gtype = 2;
				area = 80701;
				goal = 80704;
				m0fz = "1132.839966";
				m0widx = 1;
				m1ttip = 80716;
				scitem = -1;
				stype = 1;
			}
			CDboTSActSToCEvt
			{
				apptype = 0;
				cnt1 = 8;
				eitype = 0;
				idx0 = 2131111;
				cnt0 = 7;
				cnt2 = 0;
				ectype = -1;
				etype = 0;
				idx1 = 2132109;
				taid = 3;
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
	}
}

