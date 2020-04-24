CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 258;
	title = 25802;

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
			prelnk = "102;101;100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "252;1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 25807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckPCCls
			{
				clsflg = 8;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 10;
			}
			CDboTSClickNPC
			{
				npcidx = "4511403;";
			}
			CDboTSCheckClrQst
			{
				and = "256;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 25814;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 25801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511403;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 25809;
				ctype = 1;
				idx = 4511403;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 25808;
				gtype = 1;
				area = 25801;
				goal = 25804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 25805;
				stype = 1;
				taid = 1;
				title = 25802;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 25808;
			gtype = 1;
			oklnk = 2;
			area = 25801;
			goal = 25804;
			sort = 25805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 25802;
		}
		CDboTSContUsrSel
		{
			cancellnk = 4;
			cid = 3;
			lilnk = 255;
			ndesc0 = 25821;
			uspt = -1;
			desc = 25809;
			nid2 = 102;
			type = 0;
			ust = 0;
			idx = 4511403;
			ndesc2 = 25822;
			nid1 = 101;
			prelnk = "4;";
			ndesc1 = 25820;
			nid0 = 100;
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;3;";
			nolnk = 255;
			rm = 0;
			yeslnk = 3;

			CDboTSClickNPC
			{
				npcidx = "4511403;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 102;
			rwdzeny = 0;
			desc = 25831;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 25803;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511403;";
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 101;
			rwdzeny = 0;
			desc = 25830;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 25802;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511403;";
			}
		}
	}
}

