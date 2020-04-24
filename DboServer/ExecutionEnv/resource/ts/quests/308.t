CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 308;
	title = 30802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 211;
			elnk = 255;
			nextlnk = 254;
			prelnk = "102;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 3000;
			}
		}
		CDboTSContGAct
		{
			cid = 102;
			elnk = 255;
			nextlnk = 211;
			prelnk = "101;";

			CDboTSActMiniNarration
			{
				taid = 1;
				tidx = 3082;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 102;
			prelnk = "100;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 6;
			elnk = 255;
			nextlnk = 4;
			prelnk = "5;";

			CDboTSActMiniNarration
			{
				taid = 1;
				tidx = 3081;
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 255;
			nextlnk = 6;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "6;";
			nolnk = 255;
			rm = 1;
			yeslnk = 3;

			CDboTSClickNPC
			{
				npcidx = "4211101;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 5;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 30809;
				ctype = 1;
				idx = 4651205;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 30808;
				gtype = 1;
				area = 30801;
				goal = 30804;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 30805;
				stype = 2;
				taid = 1;
				title = 30802;
			}
		}
		CDboTSContUnifiedNarration
		{
			cancellnk = 100;
			cid = 3;
			idx = 3083;
			lilnk = 255;
			oklnk = 100;
			prelnk = "4;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 30814;
			nextlnk = 101;
			rwdexp = 0;
			rwdtbl = 30801;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;3;";
			usetbl = 1;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 30807;
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
				minlvl = 16;
			}
			CDboTSClickNPC
			{
				npcidx = "4651205;";
			}
			CDboTSCheckClrQst
			{
				and = "307;";
				flink = 1;
				flinknextqid = "309;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "211;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 30808;
			gtype = 1;
			oklnk = 2;
			area = 30801;
			goal = 30804;
			sort = 30805;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 30802;
		}
	}
}

