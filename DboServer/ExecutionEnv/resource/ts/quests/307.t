CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 307;
	title = 30702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 102;
			elnk = 255;
			nextlnk = 101;
			prelnk = "100;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 101;
			elnk = 255;
			nextlnk = 254;
			prelnk = "102;";

			CDboTSActMiniNarration
			{
				taid = 1;
				tidx = 3073;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 4;
			prelnk = "2;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 1000;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 100;
			prelnk = "3;";

			CDboTSActMiniNarration
			{
				taid = 1;
				tidx = 3071;
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
				conv = 30709;
				ctype = 1;
				idx = 4651102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 30708;
				gtype = 1;
				area = 30701;
				goal = 30704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 30705;
				stype = 2;
				taid = 1;
				title = 30702;
			}
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
			cont = 30708;
			gtype = 1;
			oklnk = 2;
			area = 30701;
			goal = 30704;
			sort = 30705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 30702;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 30714;
			nextlnk = 102;
			rwdexp = 0;
			rwdtbl = 30701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4651205;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 30707;
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
			CDboTSCheckClrQst
			{
				and = "306;";
				flink = 1;
				flinknextqid = "308;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4651102;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

