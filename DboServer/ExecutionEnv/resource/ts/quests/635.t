CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 635;
	title = 63502;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 63508;
				gtype = 1;
				area = 63501;
				goal = 63504;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 63505;
				stype = 2;
				taid = 1;
				title = 63502;
			}
			CDboTSActNPCConv
			{
				conv = 63509;
				ctype = 1;
				idx = 4261102;
				taid = 2;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 63514;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 63501;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1311202;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 63507;
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
				minlvl = 21;
			}
			CDboTSClickNPC
			{
				npcidx = "4261102;";
			}
			CDboTSCheckClrQst
			{
				and = "634;";
				flink = 1;
				flinknextqid = "668;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
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
			cont = 63508;
			gtype = 1;
			oklnk = 2;
			area = 63501;
			goal = 63504;
			sort = 63505;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 63502;
		}
	}
}

