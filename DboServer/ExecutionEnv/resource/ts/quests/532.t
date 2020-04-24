CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 532;
	title = 53202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 53209;
				ctype = 1;
				idx = 3273101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 53208;
				gtype = 2;
				area = 53201;
				goal = 53204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 53205;
				stype = 2;
				taid = 1;
				title = 53202;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 53214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 53201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3172104;";
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
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 53208;
			gtype = 2;
			oklnk = 2;
			area = 53201;
			goal = 53204;
			sort = 53205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 53202;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 53207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 18;
			}
			CDboTSCheckClrQst
			{
				and = "531;";
				flink = 1;
				flinknextqid = "533;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "3273101;";
			}
			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
		}
	}
}

