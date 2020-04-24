CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 115;
	title = 11502;

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
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 11507;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 10;
			}
			CDboTSClickNPC
			{
				npcidx = "7072108;";
			}
			CDboTSCheckClrQst
			{
				and = "114;";
				flink = 1;
				not = 0;
			}
			CDboTSCheckPCRace
			{
				raceflg = 1;
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
				conv = 11509;
				ctype = 1;
				idx = 7072108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 11508;
				gtype = 2;
				area = 11501;
				goal = 11504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 11505;
				stype = 64;
				taid = 1;
				title = 11502;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 11508;
			gtype = 2;
			oklnk = 2;
			area = 11501;
			goal = 11504;
			sort = 11505;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 11502;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 11514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 11501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "3;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7072108;";
			}
		}
		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSSlotMachine
			{
				st = 6;
			}
		}
	}
}

