CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 274;
	title = 27402;

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
				conv = 27409;
				ctype = 1;
				idx = 4511206;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 27408;
				gtype = 1;
				area = 27401;
				goal = 27404;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 27405;
				stype = 1;
				taid = 1;
				title = 27402;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 27407;
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
				minlvl = 12;
			}
			CDboTSClickNPC
			{
				npcidx = "4511206;";
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "278;";
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 27408;
			gtype = 1;
			oklnk = 2;
			area = 27401;
			goal = 27404;
			sort = 27405;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 27402;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 27414;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 27401;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511503;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

