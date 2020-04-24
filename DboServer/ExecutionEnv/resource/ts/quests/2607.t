CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 7;
	rq = 0;
	tid = 2607;
	title = 260702;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGCond
		{
			cid = 3;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 4;

			CDboTSClickNPC
			{
				npcidx = "1657102;";
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 255;
			nextlnk = 5;
			prelnk = "3;";

			CDboTSActPortal
			{
				dx = "0.935000";
				px = "793.625977";
				widx = -1;
				dy = "0.000000";
				py = "-95.306999";
				taid = 1;
				type = 0;
				dz = "-0.356000";
				pz = "-4424.726074";
			}
		}
		CDboTSContGAct
		{
			cid = 5;
			elnk = 252;
			nextlnk = 100;
			prelnk = "4;";

			CDboTSActWorldPlayScript
			{
				sid = 2607;
				start = 1;
				taid = 1;
				uof = 1;
			}
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
			stdiag = 260707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "1657102;";
			}
			CDboTSCheckClrQst
			{
				and = "2606;";
				flink = 1;
				flinknextqid = "2608;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 260714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 260701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "5;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1657102;";
			}
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "5;2;";
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
			cont = 260708;
			gtype = 1;
			oklnk = 2;
			area = 260701;
			goal = 260704;
			sort = 260705;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 260702;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 3;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 260709;
				ctype = 1;
				idx = 1657102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 260708;
				gtype = 1;
				area = 260701;
				goal = 260704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 260705;
				stype = 2;
				taid = 1;
				title = 260702;
			}
		}
	}
}

